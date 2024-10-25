#include "include/order_generator.h"
#include "include/hash_order.h"
#include "include/timer_loger.h"

OrderGenerator::OrderGenerator(RingBuffer<Order> &buffer, int32_t min_price,
                               int32_t max_price, int32_t N, int32_t U)
    : buffer_(buffer), min_price_(min_price), max_price_(max_price), N_(N), U_(U) {
  for (int i = 0; i < U; ++i) {
    auto hash = "auth_hash_" + std::to_string(i);
    user_auth_hashes_.push_back(hash);
  }
}

void OrderGenerator::GenerateOrder() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> price_dist(min_price_, max_price_);
  std::uniform_int_distribution<> size_dist(1, N_);
  std::uniform_int_distribution<> side_dist(0, 1);
  std::uniform_int_distribution<> auth_dist(0, U_ - 1);

  Order order;
  order.price = price_dist(gen);
  order.size = size_dist(gen);
  order.side = side_dist(gen) ? Order::Side::Buy : Order::Side::Sell;
  order.auth_hash = GenerateAuthHash(order, user_auth_hashes_[auth_dist(gen)]);
  buffer_.push(order);
}

void OrderGenerator::GenerateUntil() {
  while (!stop_gen) {
    {
      TimeLogger logger("GenerateOrder", "TimeLog.txt");
      GenerateOrder();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderGenerator::StopGenerator() { stop_gen = true; }