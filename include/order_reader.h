#pragma once

#include "hash_order.h"
#include "order_book.h"
#include "ring_buffer.hpp"

#include <__atomic/aliases.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

class OrderReader {
public:
  OrderReader(RingBuffer<Order> &buffer, OrderBook &orderBook);

  void readOrders();

  void stopReader();

private:
  std::string validateAuthHash(const Order &order);

  bool validateOrder(const Order &order);

  bool checkUserLimits(std::string user_hash, const Order &order);

  static int32_t order_count;
  RingBuffer<Order> &buffer;
  OrderBook &orderBook;
  std::atomic_bool stop_reader = false;
  std::mutex mutex;
};