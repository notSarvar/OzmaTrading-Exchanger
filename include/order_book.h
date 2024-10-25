#pragma once

#include "hash_order.h"
#include "order.h"
#include "order_logger.h"
#include "ring_buffer.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

class OrderBook {
  friend class OrderReader;

public:
  OrderBook(OrderLogger &logger, int32_t min_price, int32_t max_price,
            int32_t N, int32_t M, int32_t U)
      : logger_(logger), min_price_(min_price), max_price_(max_price), N_(N), M_(M),
        U_(U) {}

  void StopMatch();

  void AddOrder(int32_t id, const Order &order);

  void MatchUntil();

  void MatchOrders();

private:
  struct UserOrderLimits {
    int32_t buy_size = 0;
    int32_t sell_size = 0;
    int32_t orders_count = 0;
  };

  void UpdateUserLimits(const std::string &auth_hash, int32_t size, Order::Side side,
                        bool is_add = false);

  void UpdateOrders();

  std::string FindUser(const Order &order);

  std::map<int32_t, std::vector<Order>> buy_orders_;
  std::map<int32_t, std::vector<Order>> sell_orders_;
  std::vector<std::string> user_auth_hashes_;
  std::unordered_map<std::string, UserOrderLimits> user_order_limits_;
  OrderLogger &logger_;
  std::mutex mutex_;
  std::atomic_bool stop_match_ = false;
  int32_t min_price_;
  int32_t max_price_;
  int32_t N_; // max size
  int32_t M_; // max orders count
  int32_t U_; // users count
};