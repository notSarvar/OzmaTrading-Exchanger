#pragma once

#include "order.h"
#include "ring_buffer.hpp"
#include <random>
#include <thread>
#include <vector>

class OrderGenerator {
public:
  OrderGenerator(RingBuffer<Order> &buffer, int max_price,
                 int max_size, int U)
      : buffer(buffer), max_price(max_price),
        max_size(max_size), U(U) {
      for (int i = 0; i < U; ++i) {
            auto hash = "auth_hash_" + std::to_string(i);
            auth_hashes.push_back(hash);
      }
    }

  void generate();

private:
  RingBuffer<Order> &buffer;
  std::vector<std::string> auth_hashes;
  int max_price;
  int max_size;
  int U;
};
