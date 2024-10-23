#include <iostream>
#include <thread>
#include <vector>
#include <sstream>

#include "include/order_generator.h"
#include "include/order_reader.h"
#include "include/order_logger.h"

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << "<U> <N> <M> <max_price> <max_size>" << std::endl;
        return 1;
    }

    int N, M, max_price, max_size, U;
    std::istringstream(argv[1]) >> U;
    std::istringstream(argv[2]) >> N;
    std::istringstream(argv[3]) >> M;
    std::istringstream(argv[4]) >> max_price;
    std::istringstream(argv[5]) >> max_size;
    

    RingBuffer<Order> orderBuffer(100);
    RingBuffer<Order> logBuffer(100);

    OrderLogger logger(logBuffer, "orders.log");
    OrderBook orderBook(logger);

    OrderGenerator generator(orderBuffer, max_price, max_size, U);
    OrderReader reader(orderBuffer, orderBook, max_price, max_size, N, M, U);

    std::thread generatorThread(&OrderGenerator::generate, &generator);
    std::thread readerThread(&OrderReader::readOrders, &reader);
    std::thread matcherThread(&OrderBook::match, &orderBook);
    std::thread loggerThread(&OrderLogger::log, &logger);

    generatorThread.join();
    readerThread.join();
    matcherThread.join();
    loggerThread.join();

    return 0;
}