#pragma once

#include "order.h"
#include "ring_buffer.hpp"
#include <__atomic/aliases.h>
#include <random>
#include <thread>
#include <vector>

class OrderGenerator {
public:
  OrderGenerator(RingBuffer<Order> &buffer, int min_price, int max_price, int N,
                 int U);

  void generate();
  
  void stopGenerator();

private:
  RingBuffer<Order> &buffer;
  std::vector<std::string> auth_hashes;
  int min_price;
  int max_price;
  int N;
  int U;
  std::atomic_bool stop_gen = false;
};
