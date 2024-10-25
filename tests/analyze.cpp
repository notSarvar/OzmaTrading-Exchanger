#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>
#include <sstream>

#include "include/order_generator.h"
#include "include/order_reader.h"
#include "include/order_logger.h"
#include "include/order_book.h"

void measureTime(const std::string& stage, const std::function<void()>& func, std::ofstream& logFile) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    logFile << stage << ": " << duration.count() << " seconds" << std::endl;
}

int main() {
    
    int U = 2, N = 100, M = 3, min_price = 5, max_price = 100;

    RingBuffer<Order> orderBuffer(U * M);
    RingBuffer<std::string> logBuffer(U * M);

    OrderLogger logger(logBuffer, "orders.log");
    OrderBook orderBook(logger, min_price, max_price, N, M, U);

    OrderGenerator generator(orderBuffer, min_price, max_price, N, U);
    OrderReader reader(orderBuffer, orderBook);

    std::ofstream logFile("performance_log.txt");

    std::thread generatorThread([&]() {
        measureTime("Order Generation", [&]() { generator.generateOrder(); }, logFile);
    });
    
    generator.generateOrder();
    std::thread readerThread([&]() {
        measureTime("Order Reading", [&]() { reader.readOrder(); }, logFile);
    });

    std::thread matcherThread([&]() {
        measureTime("Order Matching", [&]() { orderBook.matchOrders(); }, logFile);
    });

    logFile.close();

    return 0;
}