#include "include/order_book.h"

void OrderBook::addOrder(int32_t id, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex);
  if (order.side == 1) {
    buy_orders[-order.price].push_back(order);
  } else {
    sell_orders[order.price].push_back(order);
  }
  updateUserLimits(findUser(order), -order.size, order.side, true);
}

void OrderBook::matchOrders() {
  std::lock_guard<std::mutex> lock(mutex);
  for (auto &buy_pair : buy_orders) {
    for (auto &sell_pair : sell_orders) {
      auto &buy_orders = buy_pair.second;
      auto &sell_orders = sell_pair.second;

      for (auto &buy_order : buy_orders) {
        for (auto &sell_order : sell_orders) {
          if (buy_order.price >= sell_order.price &&
              buy_order.size > 0 && sell_order.size > 0) {

            int32_t amount =
                std::min(buy_order.size, sell_order.size);
            buy_order.size -= amount;
            sell_order.size -= amount;
            logger.logMatch(buy_order, sell_order, amount);

            updateUserLimits(findUser(buy_order), amount, 1);
            updateUserLimits(findUser(sell_order), amount, 0);
          }
        }
      }
    }
  }
  updateOrders();
}

void OrderBook::match() {
  while (!stop_match) {
    matchOrders();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderBook::updateUserLimits(const std::string &auth_hash, int32_t size,
                                 int side, bool is_add) {
  auto &limits = user_order_limits[auth_hash];
  if (side == 1) { // Buy
    limits.buy_size -= size;
  } else { // Sell
    limits.sell_size -= size;
  }
  limits.orders_count += is_add ? 1 : -1;
}

std::string OrderBook::findUser(const Order &order) {
  for (const auto &user_hash : auth_hashes) {
    if (generateAuthHash(order, user_hash) == order.auth_hash) {
      return user_hash;
    }
  }
  return ""; // Invalid auth_hash should I throw an exception?
}

void OrderBook::updateOrders() {
  for (auto &buy_pair : buy_orders) {
    buy_pair.second.erase(
        std::remove_if(buy_pair.second.begin(), buy_pair.second.end(),
                       [](const Order &order) { return order.size == 0; }),
        buy_pair.second.end());
  }

  for (auto &sell_pair : sell_orders) {
    sell_pair.second.erase(
        std::remove_if(sell_pair.second.begin(), sell_pair.second.end(),
                       [](const Order &order) { return order.size == 0; }),
        sell_pair.second.end());
  }
}

void OrderBook::stopMatch() { stop_match = true; }
