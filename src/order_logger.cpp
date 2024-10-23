#include "include/order_logger.h"

void OrderLogger::log() {
    std::ofstream file(filename);
    while (true) {
      Order order;
      buffer.pop(order);
      file << "Order: " << order.price << ", " << order.size << ", "
           << order.side << ", " << order.auth_hash << "\n";
    }
  }