#pragma once

#include "order.h"
#include "ring_buffer.hpp"

class OrderReader {
public:
  OrderReader(RingBuffer<Order> &buffer, int max_price, int max_size, int N,
              int M)
      : buffer(buffer), max_price(max_price), max_size(max_size), N(N), M(M) {}

  void read() {
    while (true) {
      Order order;
      buffer.pop(order);
      if (validate(order)) {
        processOrder(order);
      }
    }
  }

private:
  bool validate(const Order &order) {
    return order.price > 0 && order.price <= max_price && order.size > 0 &&
           order.size <= max_size && (order.side == 0 || order.side == 1);
  }

  void processOrder(const Order &order) {
    // Логика обработки ордера
  }

  RingBuffer<Order> &buffer;
  int max_price;
  int max_size;
  int N;
  int M;
};
