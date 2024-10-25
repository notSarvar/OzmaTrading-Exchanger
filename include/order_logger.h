#pragma once

#include <__atomic/aliases.h>
#include <fstream>
#include <string>

#include "order.h"
#include "ring_buffer.hpp"

class OrderLogger {
public:
  OrderLogger(RingBuffer<std::string> &buffer, const std::string &filename)
      : buffer(buffer), filename(filename) {}

  void log();

  void stopLogger();

  void logMatch(const Order &buy_order, const Order &sell_order, int32_t size);

private:
  RingBuffer<std::string> &buffer;
  std::string filename;
  std::atomic_bool stop_log = false;
};
