#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "order.h"
#include "order_logger.h"
#include "ring_buffer.hpp"
#include "hash_order.h"

class OrderBook {
  friend class OrderReader;

public:
  OrderBook(OrderLogger &logger, int min_price,
              int max_price, int N, int M, int U) : logger(logger), min_price(min_price),
      max_price(max_price), N(N), M(M), U(U) {}

  void stopMatch();

  void addOrder(int32_t id, const Order &order);

  void match();

  std::string get_user(const Order &order);

  void get_orders();

private:
  struct UserOrderLimits {
    int32_t buySize = 0;
    int32_t sellSize = 0;
    int32_t ordersCount = 0;
  };

  void updateUserLimits(const std::string &auth_hash, int size, int side, bool is_add = false);

  void updateBestPrices();

  std::map<int32_t, std::vector<std::pair<int32_t, Order>>> buy_orders;
  std::map<int32_t, std::vector<std::pair<int32_t, Order>>> sell_orders;
  
  std::vector<std::string> auth_hashes;
  std::unordered_map<std::string, UserOrderLimits> userOrderLimits;
  OrderLogger &logger;
  std::mutex mutex;
  std::atomic_bool stop_match = false;
  int min_price;
  int max_price;
  int N; // max size
  int M; // max orders count
  int U; // users count
  int32_t best_bid_price = 0;  
  int32_t best_ask_price = std::numeric_limits<int32_t>::max();
};