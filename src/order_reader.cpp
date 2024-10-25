#include "include/order_reader.h"

OrderReader::OrderReader(RingBuffer<Order> &buffer, OrderBook &orderBook)
    : buffer(buffer), orderBook(orderBook) {
  for (int i = 0; i < orderBook.U; ++i) {
    auto hash = "auth_hash_" + std::to_string(i);
    orderBook.auth_hashes.push_back(hash);
    orderBook.userOrderLimits[hash] = OrderBook::UserOrderLimits();
  }
}

int32_t OrderReader::order_count = 0;

void OrderReader::readOrders() {
  while (!stop_reader) {
    Order order;
    if (buffer.pop(order)) {
      auto user = validateAuthHash(order);
      if (user != "Invalid auth_hash") {
        if (checkUserLimits(user, order) && validateOrder(order)) {
          {
            std::lock_guard<std::mutex> lock(mutex);
            switch (order.side) {
            case 0:
              orderBook.userOrderLimits[user].buySize += order.size;
              break;
            case 1:
              orderBook.userOrderLimits[user].sellSize += order.size;
              break;
            default:
              orderBook.userOrderLimits[user].ordersCount++;
            }
          }
          orderBook.addOrder(order_count++, order);
        }
      } else {
        std::cerr << "Invalid auth_hash" << std::endl;
      }
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}

std::string OrderReader::validateAuthHash(const Order &order) {
  for (const auto &user_hash : orderBook.auth_hashes) {
    if (generateAuthHash(order, user_hash) == order.auth_hash) {
      return user_hash;
    }
  }
  return "Invalid auth_hash";
}

bool OrderReader::validateOrder(const Order &order) {
  return order.price >= orderBook.min_price &&
         order.price <= orderBook.max_price && order.size > 0 &&
         order.size <= orderBook.N && (order.side == 0 || order.side == 1);
}

bool OrderReader::checkUserLimits(std::string user_hash, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex);
  auto ans = orderBook.userOrderLimits[user_hash].buySize + order.size <=
                 orderBook.N &&
             orderBook.userOrderLimits[user_hash].sellSize + order.size <=
                 orderBook.N &&
             orderBook.userOrderLimits[user_hash].ordersCount <= orderBook.M;
  return ans;
}

void OrderReader::stopReader() { stop_reader = true; }