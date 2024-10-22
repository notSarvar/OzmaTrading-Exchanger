#pragma once

#include <fstream>
#include <string>

#include "ring_buffer.hpp"
#include "order.h"

class OrderLogger {
public:
    OrderLogger(RingBuffer<Order>& buffer, const std::string& filename)
        : buffer(buffer), filename(filename) {}

    void log() {
        std::ofstream file(filename);
        while (true) {
            Order order;
            buffer.pop(order);
            file << "Order: " << order.price << ", " << order.size << ", " << order.side << ", " << order.auth_hash << "\n";
        }
    }

private:
    RingBuffer<Order>& buffer;
    std::string filename;
};
 