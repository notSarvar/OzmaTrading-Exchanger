#pragma once

#include <__atomic/aliases.h>
#include <fstream>
#include <string>
#include <thread>

#include "order.h"
#include "ring_buffer.hpp"

class OrderLogger {
public:
  OrderLogger(RingBuffer<std::string> &buffer, const std::string &filename)
      : log_buffer_(buffer), export_filename_(filename) {}

  void ExportLogUntil();

  void ExportLog();

  void StopExport();

  void LogMatched(const Order &buy_order, const Order &sell_order, int32_t size);

private:
  RingBuffer<std::string> &log_buffer_;
  std::string export_filename_;
  std::atomic_bool stop_log_ = false;
};
