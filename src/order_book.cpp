#include "include/order_book.h"
#include "include/order.h"

void OrderBook::AddOrder(int32_t id, const Order &order) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (order.side == Order::Side::Buy) {
    buy_orders_[-order.price].push_back(order);
  } else {
    sell_orders_[order.price].push_back(order);
  }
  UpdateUserLimits(FindUser(order), -order.size, order.side, true);
}

void OrderBook::MatchOrders() {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto &buy_pair : buy_orders_) {
    for (auto &sell_pair : sell_orders_) {
      auto &buy_orders = buy_pair.second;
      auto &sell_orders = sell_pair.second;

      for (auto &buy_order : buy_orders) {
        for (auto &sell_order : sell_orders) {
          if (buy_order.price >= sell_order.price &&
              buy_order.size > 0 && sell_order.size > 0) {

            int32_t amount =
                std::min(buy_order.size, sell_order.size);
            buy_order.size -= amount;
            sell_order.size -= amount;
            logger_.LogMatched(buy_order, sell_order, amount);

            UpdateUserLimits(FindUser(buy_order), amount, Order::Side::Buy);
            UpdateUserLimits(FindUser(sell_order), amount, Order::Side::Sell);
          }
        }
      }
    }
  }
  UpdateOrders();
}

void OrderBook::MatchUntil() {
  while (!stop_match_) {
    MatchOrders();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderBook::UpdateUserLimits(const std::string &auth_hash, int32_t size,
                                 Order::Side side, bool is_add) {
  auto &limits = user_order_limits_[auth_hash];
  if (side == Order::Side::Buy) { 
    limits.buy_size -= size;
  } else { 
    limits.sell_size -= size;
  }
  limits.orders_count += is_add ? 1 : -1;
}

std::string OrderBook::FindUser(const Order &order) {
  for (const auto &user_hash : user_auth_hashes_) {
    if (GenerateAuthHash(order, user_hash) == order.auth_hash) {
      return user_hash;
    }
  }
  return ""; // Invalid auth_hash should I throw an exception?
}

void OrderBook::UpdateOrders() {
  for (auto &buy_pair : buy_orders_) {
    buy_pair.second.erase(
        std::remove_if(buy_pair.second.begin(), buy_pair.second.end(),
                       [](const Order &order) { return order.size == 0; }),
        buy_pair.second.end());
  }

  for (auto &sell_pair : sell_orders_) {
    sell_pair.second.erase(
        std::remove_if(sell_pair.second.begin(), sell_pair.second.end(),
                       [](const Order &order) { return order.size == 0; }),
        sell_pair.second.end());
  }
}

void OrderBook::StopMatch() { stop_match_ = true; }
