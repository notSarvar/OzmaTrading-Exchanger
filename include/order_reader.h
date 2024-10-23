#pragma once

#include "hash_order.h"
#include "order_book.h"
#include "ring_buffer.hpp"

#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

class OrderReader {
public:
  OrderReader(RingBuffer<Order> &buffer, OrderBook &orderBook, int max_price,
              int max_size, int N, int M, int U)
      : buffer(buffer), orderBook(orderBook), max_price(max_price),
        max_size(max_size), N(N), M(M), U(U) {
    for (int i = 0; i < U; ++i) {
      auto hash = "auth_hash_" + std::to_string(i);
      orderBook.auth_hashes.push_back(hash);
      orderBook.userOrderLimits[hash] = OrderBook::UserOrderLimits();
      }
    }

  void readOrders();

private:
  std::string validateAuthHash(const Order &order);

  bool validateOrder(const Order &order);

  bool checkUserLimits(std::string user_hash, const Order &order);

  static int32_t order_count;
  RingBuffer<Order> &buffer;
  OrderBook &orderBook;
  int max_price;
  int max_size;
  int N; // max size
  int M; // max orders count
  int U; // users count

  std::mutex mutex;
};