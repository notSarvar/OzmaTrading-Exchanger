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

  void Read();

  void ReadOrder();

  void StopReader();

private:
  std::string ValidateAuthHash(const Order &order);

  bool ValidateOrder(const Order &order);

  bool CheckUserLimits(std::string user_hash, const Order &order);

  static int32_t order_count_;
  RingBuffer<Order> &order_buffer_;
  OrderBook &order_book_;
  std::atomic_bool stop_reader_ = false;
  std::mutex mutex_;
};