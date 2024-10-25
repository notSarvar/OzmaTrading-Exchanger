#include "include/order_reader.h"
#include "include/order.h"
#include "include/timer_loger.h"

OrderReader::OrderReader(RingBuffer<Order> &buffer, OrderBook &orderBook)
    : order_buffer_(buffer), order_book_(orderBook) {
  for (int i = 0; i < orderBook.U_; ++i) {
    auto hash = "auth_hash_" + std::to_string(i);
    orderBook.user_auth_hashes_.push_back(hash);
    orderBook.user_order_limits_[hash] = OrderBook::UserOrderLimits();
  }
}

int32_t OrderReader::order_count_ = 0;

void OrderReader::ReadOrder() {
  Order order;
  if (order_buffer_.pop(order)) {
    std::string user = "";
    {
      TimeLogger logger("ValidateAuthHash", "TimeLog.txt");
      user = ValidateAuthHash(order);
    }
    if (!user.empty()) {
      bool check_limits = false;
      bool validate_order = false;
     {
      TimeLogger logger("ValidateOrder", "TimeLog.txt");
      check_limits = CheckUserLimits(user, order);
      validate_order = ValidateOrder(order);
     }

      if (CheckUserLimits(user, order) && ValidateOrder(order)) {
        {
          std::lock_guard<std::mutex> lock(mutex_);
          switch (order.side) {
          case Order::Side::Buy:
            order_book_.user_order_limits_[user].buy_size += order.size;
            break;
          case Order::Side::Sell:
            order_book_.user_order_limits_[user].sell_size += order.size;
            break;
          default:
            order_book_.user_order_limits_[user].orders_count++;
          }
        }
        {
          TimeLogger logger("AddOrder", "TimeLog.txt");
          order_book_.AddOrder(order_count_++, order);
        }
      }
    } else {
      std::cerr << "Invalid auth_hash" << std::endl;
    }
  } 
}

void OrderReader::Read() {
  while (!stop_reader_) {
  {
    TimeLogger logger("ReadOrder", "TimeLog.txt");
    ReadOrder();
  }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

std::string OrderReader::ValidateAuthHash(const Order &order) {
  for (const auto &user_hash : order_book_.user_auth_hashes_) {
    if (GenerateAuthHash(order, user_hash) == order.auth_hash) {
      return user_hash;
    }
  }
  return ""; // Invalid auth_hash should I throw an exception?
}

bool OrderReader::ValidateOrder(const Order &order) {
  return order.price >= order_book_.min_price_ &&
         order.price <= order_book_.max_price_ && order.size > 0 &&
         order.size <= order_book_.N_ && (order.side == Order::Side::Buy || order.side == Order::Side::Sell); 
}

bool OrderReader::CheckUserLimits(std::string user_hash, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto ans =
      order_book_.user_order_limits_[user_hash].buy_size + order.size <=
          order_book_.N_ &&
      order_book_.user_order_limits_[user_hash].sell_size + order.size <=
          order_book_.N_ &&
      order_book_.user_order_limits_[user_hash].orders_count <= order_book_.M_;
  return ans;
}

void OrderReader::StopReader() { stop_reader_ = true; }