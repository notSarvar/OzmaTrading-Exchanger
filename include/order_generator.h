#pragma once

#include "order.h"
#include "ring_buffer.hpp"
#include <random>
#include <thread>

class OrderGenerator {
public:
  OrderGenerator(RingBuffer<Order> &buffer, int max_price, int max_size, int U)
      : buffer(buffer), max_price(max_price), max_size(max_size), U(U) {}

  void generate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> price_dist(1, max_price);
    std::uniform_int_distribution<> size_dist(1, max_size);
    std::uniform_int_distribution<> side_dist(0, 1);

    while (true) {
      for (int i = 0; i < U; ++i) {
        Order order = {price_dist(gen), size_dist(gen), side_dist(gen),
                       "auth_hash_" + std::to_string(i)};
        buffer.push(order);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
  }

private:
  RingBuffer<Order> &buffer;
  int max_price;
  int max_size;
  int U;
};
