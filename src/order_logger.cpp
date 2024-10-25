#include "include/order_logger.h"

void OrderLogger::logOrder() {
  std::string log;
  if (buffer.pop(log)) {
    std::ofstream file(filename, std::ios::app);
    file << log << std::endl;
  }
}

void OrderLogger::log() {
  while (!stop_log) {
    logOrder();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderLogger::logMatch(const Order &buy_order, const Order &sell_order,
                           int32_t size) {
  std::string log =
      "Matched{\n Buy order:\n\tPrice: " + std::to_string(buy_order.price) + "\n\tSize left: " + std::to_string(buy_order.size) +
      " With hash: " + buy_order.auth_hash +
      "\n Sell order:\n\tPrice: " + std::to_string(sell_order.price) +"\n\tSize left: " + std::to_string(sell_order.size) +
      " With hash: " + sell_order.auth_hash +
      " \n Amount: " + std::to_string(size) + "\n}";
  buffer.push(log);
}

void OrderLogger::stopLogger() { stop_log = true; }