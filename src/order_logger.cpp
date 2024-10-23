#include "include/order_logger.h"

void OrderLogger::log() {
  while (true) {
    std::string log;
    if (buffer.pop(log)) {
      std::ofstream file(filename, std::ios::app);
      file << log << std::endl;
    }
  }
}

void OrderLogger::logMatch(const Order &buy_order, const Order &sell_order) {
  std::string log = "Matched: " + std::to_string(buy_order.price) + " " +
                    std::to_string(buy_order.size) + " " +
                    std::to_string(sell_order.price) + " " +
                    std::to_string(sell_order.size);
  buffer.push(log);
}