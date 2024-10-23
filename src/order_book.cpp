#include "include/order_book.h"

void OrderBook::addOrder(int32_t id, const Order &order) {
  std::cout << "Adding order: " << id << " " << order.price << " " << order.size
            << " " << order.side << " " << order.auth_hash << std::endl;
  std::lock_guard<std::mutex> lock(mutex);
  if (order.side == 0) {
    buy_orders[order.price].insert(std::make_pair(id, order));
  } else {
    sell_orders[order.price].insert(std::make_pair(id, order));
  }
}

void OrderBook::match() {
  while (true) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      
      for (auto &buy_pair : buy_orders) {
        for (auto &sell_pair : sell_orders) {
          auto buy_order_set = buy_pair.second;
          auto sell_order_set = sell_pair.second;

          for (const auto &buy_order_pair : buy_order_set) {
            for (const auto &sell_order_pair : sell_order_set) {
              if (buy_order_pair.first >=
                  sell_order_pair.first) { 
                logger.logMatch(buy_order_pair.second, sell_order_pair.second);
                
                sell_orders[sell_pair.first].erase(sell_order_pair);
                buy_orders[buy_pair.first].erase(buy_order_pair);
                break; 
              }
            }
          }
        }
      }
    }
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)); 
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