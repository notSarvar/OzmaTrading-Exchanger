#include <iostream>
#include <thread>

#include "include/order_generator.h"
#include "include/order_reader.h"
#include "include/order_logger.h"
#include "include/matcher.h"

int main(int argc, char** argv) {
    RingBuffer<Order> orderBuffer(100);
    RingBuffer<Order> logBuffer(100);

    OrderGenerator generator(orderBuffer, 100, 20, 20);
    OrderReader reader(orderBuffer, 100, 20, 20, 10);
    OrderLogger logger(logBuffer, "orders.log");

    std::thread generatorThread(&OrderGenerator::generate, &generator);
    std::thread readerThread(&OrderReader::read, &reader);
    std::thread loggerThread(&OrderLogger::log, &logger);

    generatorThread.join();
    readerThread.join();
    loggerThread.join();

    return 0;
}