#include "include/order_book.h"

void OrderBook::addOrder(int32_t id, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex);
  if (order.side == 1) {
    buy_orders[-order.price].push_back(std::make_pair(id, order));
  } else {
    sell_orders[order.price].push_back(std::make_pair(id, order));
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
              if (buy_order_pair.second.price >= sell_order_pair.second.price &&
                  buy_order_pair.second.size > 0 &&
                  sell_order_pair.second.size > 0) {

                int32_t size = std::min(buy_order_pair.second.size,
                                        sell_order_pair.second.size);
                buy_order_pair.second.size -= size;
                sell_order_pair.second.size -= size;
                logger.logMatch(buy_order_pair.second, sell_order_pair.second,
                                size);

                updateUserLimits(get_user(buy_order_pair.second),
                                 size, 1);
                updateUserLimits(get_user(sell_order_pair.second),
                                 size, 0);
              }
            }
          }
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderBook::updateUserLimits(const std::string &auth_hash, int size,
                                 int side, bool is_add) {
  auto &limits = userOrderLimits[auth_hash];
  if (side == 1) { // Buy
    limits.buySize -= size;
  } else { // Sell
    limits.sellSize -= size;
  }
  limits.ordersCount += is_add ? 1 : -1;
}

std::string OrderBook::get_user(const Order &order) {
  for (const auto &user_hash : auth_hashes) {
    if (generateAuthHash(order, user_hash) == order.auth_hash) {
      return user_hash;
    }
  }
  return "";
}

void OrderBook::get_orders() {
  std::lock_guard<std::mutex> lock(mutex);
  for (const auto &buy_pair : buy_orders) {
    for (const auto &buy_order_pair : buy_pair.second) {
      std::cout << "Buy order: " << buy_order_pair.first
                << " Price: " << buy_order_pair.second.price
                << " Amount: " << buy_order_pair.second.size << std::endl;
    }
  }

  for (const auto &sell_pair : sell_orders) {
    for (const auto &sell_order_pair : sell_pair.second) {
      std::cout << "Sell order: " << sell_order_pair.first
                << " Price: " << sell_order_pair.second.price
                << " Amount: " << sell_order_pair.second.size << std::endl;
    }
  }
}

void OrderBook::stopMatch() { stop_match = true; }
