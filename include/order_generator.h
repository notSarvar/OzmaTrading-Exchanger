#pragma once

#include "order.h"
#include "ring_buffer.hpp"
#include <random>
#include <thread>
#include <vector>

class OrderGenerator {
public:
  OrderGenerator(RingBuffer<Order> &buffer, int max_price,
                 int max_size, int U);

  void generate();

private:
  RingBuffer<Order> &buffer;
  std::vector<std::string> auth_hashes;
  int max_price;
  int max_size;
  int U;
};
