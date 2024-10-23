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