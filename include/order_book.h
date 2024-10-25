#pragma once

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

#include "hash_order.h"
#include "order.h"
#include "order_logger.h"
#include "ring_buffer.hpp"

class OrderBook {
  friend class OrderReader;

public:
  OrderBook(OrderLogger &logger, int32_t min_price, int32_t max_price,
            int32_t N, int32_t M, int32_t U)
      : logger(logger), min_price(min_price), max_price(max_price), N(N), M(M),
        U(U) {}

  void stopMatch();

  void addOrder(int32_t id, const Order &order);

  void match();

  void matchOrders();

private:
  struct UserOrderLimits {
    int32_t buy_size = 0;
    int32_t sell_size = 0;
    int32_t orders_count = 0;
  };

  void updateUserLimits(const std::string &auth_hash, int32_t size, int side,
                        bool is_add = false);

  void updateOrders();

  std::string findUser(const Order &order);

  std::map<int32_t, std::vector<Order>> buy_orders;
  std::map<int32_t, std::vector<Order>> sell_orders;
  std::vector<std::string> auth_hashes;
  std::unordered_map<std::string, UserOrderLimits> user_order_limits;
  OrderLogger &logger;
  std::mutex mutex;
  std::atomic_bool stop_match = false;
  int32_t min_price;
  int32_t max_price;
  int32_t N; // max size
  int32_t M; // max orders count
  int32_t U; // users count
  int32_t best_bid_price = 0;
  int32_t best_ask_price = std::numeric_limits<int32_t>::max();
};