#pragma once

#include <string>

struct Order {
  int32_t price;
  int32_t size;
  int side; // 1 - Buy, 0 - Sell
  std::string auth_hash;

  bool operator==(const Order& other) const {
    return price == other.price &&
               size == other.size &&
               side == other.side &&
               auth_hash == other.auth_hash;
  }
};

namespace std {
    template <>
    struct hash<Order> {
        std::size_t operator()(const Order& order) const {
            std::size_t h1 = std::hash<int32_t>{}(order.price);
            std::size_t h2 = std::hash<int32_t>{}(order.size);
            std::size_t h3 = std::hash<int>{}(order.side);
            std::size_t h4 = std::hash<std::string>{}(order.auth_hash);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1); 
    }
};
