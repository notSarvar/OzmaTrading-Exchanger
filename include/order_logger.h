#pragma once

#include <fstream>
#include <string>

#include "order.h"
#include "ring_buffer.hpp"

class OrderLogger {
  friend class OrderBook;

public:
  OrderLogger(RingBuffer<Order> &buffer, const std::string &filename)
        : buffer(buffer), filename(filename) {}

  void log();

private:
  RingBuffer<Order> &buffer;
    std::string filename;
};
 