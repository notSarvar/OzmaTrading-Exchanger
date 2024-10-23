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

class OrderBook {
  friend class OrderReader;

public:
  OrderBook(OrderLogger &logger) : logger(logger) {}

  void addOrder(int32_t id, const Order &order);

  void match();

private:
  struct UserOrderLimits {
    int buySize = 0;
    int sellSize = 0;
    std::unordered_set<int32_t> buy_orders;
    std::unordered_set<int32_t> sell_orders;
  };

  void updateUserLimits(const std::string &auth_hash, int size, int side);

  std::unordered_map<int32_t, std::pair<int32_t, Order>> buy_orders;
  std::unordered_map<int32_t, std::pair<int32_t, Order>> sell_orders;
  std::vector<std::string> auth_hashes;
  std::unordered_map<std::string, UserOrderLimits> userOrderLimits;
  OrderLogger &logger;
  std::mutex mutex;
};