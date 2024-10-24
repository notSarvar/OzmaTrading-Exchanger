#include "include/order_book.h"

void OrderBook::addOrder(int32_t id, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex);
  if (order.side == 1) {
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
                
                updateUserLimits(get_user(buy_order_pair.first),
                                 buy_order_pair.second.size, 1);
                updateUserLimits(get_user(sell_order_pair.first),
                                  sell_order_pair.second.size, 0);  
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

std::string OrderBook::get_user(int32_t order_id) {
  std::lock_guard<std::mutex> lock(mutex);
  for (const auto &user : userOrderLimits) {
    if (user.second.buy_orders.find(order_id) !=
            user.second.buy_orders.end() ||
        user.second.sell_orders.find(order_id) !=
            user.second.sell_orders.end()) {
      return user.first;
    }
  }
  return "";
}

void OrderBook::get_orders() {
  std::lock_guard<std::mutex> lock(mutex);
  for (const auto &buy_pair : buy_orders) {
    for (const auto &buy_order_pair : buy_pair.second) {
      std::cout << "Buy order: " << buy_order_pair.first << " Price: "
                << buy_order_pair.second.price << " Amount: "
                << buy_order_pair.second.size << std::endl;
    }
  }

  for (const auto &sell_pair : sell_orders) {
    for (const auto &sell_order_pair : sell_pair.second) {
      std::cout << "Sell order: " << sell_order_pair.first << " Price: "
                << sell_order_pair.second.price << " Amount: "
                << sell_order_pair.second.size << std::endl;
    }
  }
}