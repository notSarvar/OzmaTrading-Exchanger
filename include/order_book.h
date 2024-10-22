#pragma once

#include <algorithm>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "order.h"
#include "ring_buffer.hpp"

class OrderBook {
public:
  OrderBook(size_t max_size) : buy_orders(max_size), sell_orders(max_size) {}

  void addOrder(const Order &order) {
    std::lock_guard<std::mutex> lock(mutex);
    if (order.side == 1) { // Buy
      buy_orders.push(order);
    } else { // Sell
      sell_orders.push(order);
    }
    matchOrders();
  }

  std::vector<Order> getOrders() {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<Order> all_orders;
    Order order;
    while (buy_orders.pop(order)) {
      all_orders.push_back(order);
    }
    while (sell_orders.pop(order)) {
      all_orders.push_back(order);
    }
    return all_orders;
  }

private:
  void matchOrders() {
    while (!buy_orders.empty() && !sell_orders.empty()) {
      Order buy_order, sell_order;
      if (buy_orders.pop(buy_order) && sell_orders.pop(sell_order)) {
        if (buy_order.price >= sell_order.price) {
          int matched_size = std::min(buy_order.size, sell_order.size);
          buy_order.size -= matched_size;
          sell_order.size -= matched_size;

          std::cout << "Matched: Buy " << buy_order.price << " with Sell "
                    << sell_order.price << " for size " << matched_size
                    << std::endl;

          if (buy_order.size > 0) {
            buy_orders.push(buy_order);
          }
          if (sell_order.size > 0) {
            sell_orders.push(sell_order);
          }
        } else {
          buy_orders.push(buy_order);
          sell_orders.push(sell_order);
          break;
        }
      }
    }
  }

  RingBuffer<Order> buy_orders;
  RingBuffer<Order> sell_orders;
  std::mutex mutex;
};