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

  void generate();

  void stopGenerator();

  void generateOrder();

private:
  RingBuffer<Order> &buffer;
  std::vector<std::string> auth_hashes;
  int32_t min_price;
  int32_t max_price;
  int32_t N;
  int32_t U;
  std::atomic_bool stop_gen = false;
};
