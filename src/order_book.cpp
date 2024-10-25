#include "include/order_book.h"

void OrderBook::addOrder(int32_t id, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex);
  if (order.side == 1) {
    buy_orders[-order.price].push_back(order);
  } else {
    sell_orders[order.price].push_back(order);
  }
  updateUserLimits(get_user(order), -order.size, order.side, true);
}

void OrderBook::match() {
  while (!stop_match) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      for (auto &buy_pair : buy_orders) {
        for (auto &sell_pair : sell_orders) {
          auto &buy_order_set = buy_pair.second;
          auto &sell_order_set = sell_pair.second;

          for (auto &buy_order_pair : buy_order_set) {
            for (auto &sell_order_pair : sell_order_set) {
              if (buy_order_pair.price >= sell_order_pair.price &&
                  buy_order_pair.size > 0 && sell_order_pair.size > 0) {

                int32_t amount =
                    std::min(buy_order_pair.size, sell_order_pair.size);
                buy_order_pair.size -= amount;
                sell_order_pair.size -= amount;
                logger.logMatch(buy_order_pair, sell_order_pair, amount);

                updateUserLimits(get_user(buy_order_pair), amount, 1);
                updateUserLimits(get_user(sell_order_pair), amount, 0);
              }
            }
          }
        }
      }
    }
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

std::string OrderBook::get_user(const Order &order) {
  for (const auto &user_hash : auth_hashes) {
    if (generateAuthHash(order, user_hash) == order.auth_hash) {
      return user_hash;
    }
  }
  return ""; // Invalid auth_hash should I throw an exception?
}

void OrderBook::stopMatch() { stop_match = true; }
