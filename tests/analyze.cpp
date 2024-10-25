#include "include/order_generator.h"
#include "include/order_reader.h"
#include "include/order_logger.h"
#include "include/order_book.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <sys/_types/_int32_t.h>
#include <thread>
#include <vector>
#include <sstream>

void measureTime(const std::string& stage, const std::function<void()>& func, std::ofstream& logFile, int32_t cnt = 1) {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < cnt; ++i) {
        func();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    logFile << stage << ": " << duration.count() << " seconds" << std::endl;
}

void SingleTestCase(std::ofstream& logFile) {
    RingBuffer<Order> orderBuffer(4);
    RingBuffer<std::string> logBuffer(10);

    OrderLogger logger(logBuffer, "analyze_orders.log");
    OrderBook orderBook(logger, 0, 100, 4, 1, 2);

    OrderGenerator generator(orderBuffer, 0, 100, 4, 2);
    OrderReader reader(orderBuffer, orderBook);

    measureTime("Order Generation", [&]() { generator.GenerateOrder(); }, logFile);
    generator.GenerateOrder();
    measureTime("Order Reading, auth and adding to OrderBook", [&]() { reader.ReadOrder(); }, logFile);
    measureTime("Order Matching", [&]() { orderBook.MatchOrders(); }, logFile);
    measureTime("Order Logging", [&]() { logger.ExportLog(); }, logFile);
}

void TestCase(int32_t U, int32_t N, int32_t M, int32_t min_price, int32_t max_price, std::ofstream& logFile, std::string filename = "analyze_orders.log") {
    RingBuffer<Order> orderBuffer(U * M);
    RingBuffer<std::string> logBuffer(U * M);

    OrderLogger logger(logBuffer, filename);
    OrderBook orderBook(logger, min_price, max_price, N, M, U);

    OrderGenerator generator(orderBuffer, min_price, max_price, N, U);
    OrderReader reader(orderBuffer, orderBook);

    std::thread generatorThread([&]() {
        measureTime("Order Generation", [&]() { generator.GenerateOrder(); }, logFile, U * M);
    });
    
    std::thread readerThread([&]() {
        measureTime("Order Reading", [&]() { reader.ReadOrder(); }, logFile, U * M);
    });

    std::thread matcherThread([&]() {
        measureTime("Order Matching", [&]() { orderBook.MatchOrders(); }, logFile, U * M);
    });

    std::thread loggerThread([&]() {
        measureTime("Order Logging", [&]() { logger.ExportLog(); }, logFile,  U * M);
    });

    generatorThread.join();
    readerThread.join();
    matcherThread.join();
    loggerThread.join();
}

int main() {
    std::ofstream logFile("performance_log.txt");
    logFile << "Small case with few users and orders" << std::endl;
    SingleTestCase(logFile);
    logFile << "Bigger case with many users and orders" << std::endl;

    int32_t U = 100, N = 100, M = 10, min_price = 5, max_price = 100;
    TestCase(U, N, M, min_price, max_price, logFile);

    logFile.close();

    return 0;
}