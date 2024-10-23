#include "include/hash_order.h"

std::string generateAuthHash(const Order& order, const std::string& userKey) {
    std::string orderData = std::to_string(order.price) + std::to_string(order.size) + std::to_string(order.side);
    
    const uint64_t fnv_prime = 1099511628211u;
    const uint64_t offset_basis = 14695981039346656037u;
    uint64_t hash = offset_basis;

    auto hash_combine = [&hash, fnv_prime](const std::string& data) {
        for (char c : data) {
            hash ^= static_cast<uint64_t>(c);
            hash *= fnv_prime;
        }
    };

    hash_combine(userKey);
    hash_combine(orderData);

    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
  }