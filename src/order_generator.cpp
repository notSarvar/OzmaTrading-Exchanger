#include "include/order_generator.h"
#include "include/hash_order.h"

void OrderGenerator::generate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> price_dist(1, max_price);
    std::uniform_int_distribution<> size_dist(1, max_size);
    std::uniform_int_distribution<> side_dist(0, 1);
    std::uniform_int_distribution<> auth_dist(0, U - 1);

    while (true) {
      Order order;
      order.price = price_dist(gen);
      order.size = size_dist(gen);
      order.side = side_dist(gen);
      order.auth_hash = generateAuthHash(order, auth_hashes[auth_dist(gen)]);
      buffer.push(order);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}