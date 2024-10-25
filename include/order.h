#pragma once

#include <string>

struct Order {
  enum class Side {
    Buy = 1,
    Sell = 0,
  };

  int32_t price;
  int32_t size;
  Side side;
  std::string auth_hash;

  bool operator==(const Order &other) const {
    return price == other.price && size == other.size && side == other.side &&
           auth_hash == other.auth_hash;
  }
};
