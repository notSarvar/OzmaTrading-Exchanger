#include "include/order_logger.h"
#include <string>

void OrderLogger::log() {
  while (!stop_log) {
    std::string log;
    if (buffer.pop(log)) {
      std::ofstream file(filename, std::ios::app);
      file << log << std::endl;
    }
  }
}

void OrderLogger::logMatch(const Order &buy_order, const Order &sell_order,
                           int32_t size) {
  std::string log =
      "Matched{\n Buy order:\nPrice: " + std::to_string(buy_order.price) +
      " With hash: " + buy_order.auth_hash +
      "\nSell order:\nPrice: " + std::to_string(sell_order.price) +
      " With hash: " + sell_order.auth_hash +
      " \nAmount: " + std::to_string(size) + "\n}";
  buffer.push(log);
}

void OrderLogger::stopLogger() { stop_log = true; }