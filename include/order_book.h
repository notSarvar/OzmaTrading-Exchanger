#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "order.h"
#include "order_logger.h"
#include "ring_buffer.hpp"

namespace std {
    template <>
    struct hash<Order> {
        std::size_t operator()(const Order& order) const {
            std::size_t h1 = std::hash<int32_t>{}(order.price);
            std::size_t h2 = std::hash<int32_t>{}(order.size);
            std::size_t h3 = std::hash<int>{}(order.side);
            std::size_t h4 = std::hash<std::string>{}(order.auth_hash);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1); 
    }
};


class OrderBook {
  friend class OrderReader;

public:
  OrderBook(OrderLogger &logger) : logger(logger) {}

  void addOrder(int32_t id, const Order &order);

  void match();

  std::string get_user(int32_t order_id);

  void get_orders();

private:
  struct UserOrderLimits {
    int buySize = 0;
    int sellSize = 0;
    std::unordered_set<int32_t> buy_orders;
    std::unordered_set<int32_t> sell_orders;
  };

  void updateUserLimits(const std::string &auth_hash, int size, int side);

  std::unordered_map<int32_t, std::unordered_set<std::pair<int32_t, Order>, pair_hash>> buy_orders;
  std::unordered_map<int32_t, std::unordered_set<std::pair<int32_t, Order>, pair_hash>> sell_orders;
  
  std::vector<std::string> auth_hashes;
  std::unordered_map<std::string, UserOrderLimits> userOrderLimits;
  OrderLogger &logger;
  std::mutex mutex;
};