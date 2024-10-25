#pragma once

#include "order.h"
#include "ring_buffer.hpp"
#include <__atomic/aliases.h>
#include <random>
#include <thread>
#include <vector>

class OrderGenerator {
public:
  OrderGenerator(RingBuffer<Order> &buffer, int32_t min_price,
                 int32_t max_price, int32_t N, int32_t U);

  void GenerateUntil();

  void StopGenerator();

  void GenerateOrder();

private:
  RingBuffer<Order> &buffer_;
  std::vector<std::string> user_auth_hashes_;
  int32_t min_price_;
  int32_t max_price_;
  int32_t N_;
  int32_t U_;
  std::atomic_bool stop_gen = false;
};
