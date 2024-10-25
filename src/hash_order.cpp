#include "include/hash_order.h"

std::string generateAuthHash(const Order &order, const std::string &userKey) {
    std::string orderData = std::to_string(order.price) +
                            std::to_string(order.size) +
                            std::to_string(order.side);

    std::hash<std::string> hasher;
    size_t hash = hasher(userKey) ^ (hasher(orderData) << 1);

    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;

    return ss.str();
}