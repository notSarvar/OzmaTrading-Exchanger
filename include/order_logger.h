#pragma once

#include <fstream>
#include <string>

#include "order.h"
#include "ring_buffer.hpp"

class OrderLogger {
  friend class OrderBook;

public:
  OrderLogger(RingBuffer<std::string> &buffer, const std::string &filename)
        : buffer(buffer), filename(filename) {}

  void log();

  void logMatch(const Order &buy_order, const Order &sell_order);

private:
  RingBuffer<std::string> &buffer;
    std::string filename;
};
 