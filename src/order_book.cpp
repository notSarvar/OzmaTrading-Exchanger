#include "include/order_book.h"

void OrderBook::addOrder(int32_t id, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex);
  if (order.side == 0) {
    buy_orders[order.price] = std::make_pair(id, order);
  } else {
    sell_orders[order.price] = std::make_pair(id, order);
  }
}

void OrderBook::match() {
  while (true) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      while (!buy_orders.empty() && !sell_orders.empty()) {
        auto buy_it = buy_orders.begin();
        auto sell_it = sell_orders.begin();

        if (buy_it->first >= sell_it->first) {
          Order &buy_order = buy_it->second.second;
          Order &sell_order = sell_it->second.second;
          logger.logMatch(buy_order, sell_order);
          int matched_size = std::min(buy_order.size, sell_order.size);
          buy_order.size -= matched_size;
          sell_order.size -= matched_size;

          updateUserLimits(buy_order.auth_hash, matched_size, 1);
          updateUserLimits(sell_order.auth_hash, matched_size, 0);

          if (buy_order.size == 0) {
            buy_orders.erase(buy_it);
          }
          if (sell_order.size == 0) {
            sell_orders.erase(sell_it);
          }
        } else {
          break;
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderBook::updateUserLimits(const std::string &auth_hash, int size,
                                 int side) {
  std::lock_guard<std::mutex> lock(mutex);
  auto &limits = userOrderLimits[auth_hash];
  if (side == 1) { // Buy
    limits.buySize -= size;
  } else { // Sell
    limits.sellSize -= size;
  }
}