#include <iostream>
#include "include/order.h"
#include "include/order_book.h"
#include "include/order_logger.h"
#include "include/order_reader.h"
#include "include/hash_order.h"

void testOrderBook() {
    RingBuffer<Order> orderBuffer(5);
    RingBuffer<std::string> logBuffer(5);

    OrderLogger logger(logBuffer, "test_orders.log");
    OrderBook orderBook(logger, 0, 200, 100, 100, 100);

    OrderReader reader(orderBuffer, orderBook);

    // Добавляем ордера вручную
    Order buyOrder1 = {100, 10, Order::Side::Buy, "user1"};
    auto hash = GenerateAuthHash(buyOrder1, "auth_hash_0");
    buyOrder1.auth_hash = hash;

    Order buyOrder2 = {105, 5, Order::Side::Buy, "user2"};
    hash = GenerateAuthHash(buyOrder1, "auth_hash_1");
    buyOrder2.auth_hash = hash;
    
    Order sellOrder1 = {95, 8, Order::Side::Sell, "user3"};
    hash = GenerateAuthHash(sellOrder1, "auth_hash_2");
    sellOrder1.auth_hash = hash;

    Order sellOrder2 = {100, 7, Order::Side::Sell, "user4"};
    hash = GenerateAuthHash(sellOrder1, "auth_hash_3");
    sellOrder2.auth_hash = hash;
    orderBook.AddOrder(0, buyOrder1);
    orderBook.AddOrder(1, buyOrder2);
    orderBook.AddOrder(2, sellOrder1);
    orderBook.AddOrder(3, sellOrder2);
    std::thread readerThread(&OrderReader::ReadOrder, &reader);
    std::thread matcherThread(&OrderBook::MatchUntil, &orderBook);
    std::thread loggerThread(&OrderLogger::ExportLogUntil, &logger);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    reader.StopReader();
    orderBook.StopMatch();
    logger.StopExport();

    readerThread.join();
    matcherThread.join();
    loggerThread.join();
}

int main() {
    testOrderBook();
    return 0;
}