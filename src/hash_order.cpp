#include "include/hash_order.h"

std::string generateAuthHash(const Order& order, const std::string& userKey) {
    std::string orderData = std::to_string(order.price) + std::to_string(order.size) + std::to_string(order.side);
    
    unsigned char* digest;
    digest = HMAC(EVP_sha256(), userKey.c_str(), userKey.length(), 
                  (unsigned char*)orderData.c_str(), orderData.length(), nullptr, nullptr);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    
    return ss.str();
  }