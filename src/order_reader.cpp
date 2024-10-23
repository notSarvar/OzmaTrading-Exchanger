#include "include/order_reader.h"

OrderReader::OrderReader(RingBuffer<Order> &buffer, OrderBook &orderBook,
                         int min_price, int max_price, int N, int M, int U)
    : buffer(buffer), orderBook(orderBook), min_price(min_price),
      max_price(max_price), N(N), M(M), U(U) {
  for (int i = 0; i < U; ++i) {
    auto hash = "auth_hash_" + std::to_string(i);
    orderBook.auth_hashes.push_back(hash);
    orderBook.userOrderLimits[hash] = OrderBook::UserOrderLimits();
  }
}

int32_t OrderReader::order_count = 0;

void OrderReader::readOrders() {
  while (true) {
    Order order;
    if (buffer.pop(order)) {
      try {
        auto user = validateAuthHash(order);
        if (checkUserLimits(user, order) && validateOrder(order)) {
          {
            std::lock_guard<std::mutex> lock(mutex);
            switch (order.side) {
            case 0:
              orderBook.userOrderLimits[user].buySize += order.size;
              orderBook.userOrderLimits[user].buy_orders.insert(order_count);
              break;
            case 1:
              orderBook.userOrderLimits[user].sellSize += order.size;
              orderBook.userOrderLimits[user].sell_orders.insert(order_count);
              break;
            }
          }
          orderBook.addOrder(order_count++, order);
        }
      } catch (const char *msg) {
        std::cerr << msg << std::endl;
        continue;
      }
    }
  }
}

std::string OrderReader::validateAuthHash(const Order &order) {
  for (const auto &hash : orderBook.auth_hashes) {
    if (generateAuthHash(order, hash) == order.auth_hash) {
      return hash;
    }
  }
  throw("Invalid auth_hash");
}

bool OrderReader::validateOrder(const Order &order) {
  return order.price >= min_price && order.price <= max_price &&
         order.size > 0 && order.size <= N &&
         (order.side == 0 || order.side == 1);
}

bool OrderReader::checkUserLimits(std::string user_hash, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex);
  auto ans = orderBook.userOrderLimits[user_hash].buySize + order.size <= N &&
             orderBook.userOrderLimits[user_hash].sellSize + order.size <= N &&
             orderBook.userOrderLimits[user_hash].buy_orders.size() +
                     orderBook.userOrderLimits[user_hash].sell_orders.size() <=
                 M;
  return ans;
}