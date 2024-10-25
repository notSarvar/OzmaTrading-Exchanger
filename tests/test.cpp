#include <iostream>
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
    Order buyOrder1 = {100, 10, 1, "user1"};
    auto hash = generateAuthHash(buyOrder1, "auth_hash_0");
    buyOrder1.auth_hash = hash;

    Order buyOrder2 = {105, 5, 1, "user2"};
    hash = generateAuthHash(buyOrder1, "auth_hash_1");
    buyOrder2.auth_hash = hash;
    
    Order sellOrder1 = {95, 8, 0, "user3"};
    hash = generateAuthHash(sellOrder1, "auth_hash_2");
    sellOrder1.auth_hash = hash;

    Order sellOrder2 = {100, 7, 0, "user4"};
    hash = generateAuthHash(sellOrder1, "auth_hash_3");
    sellOrder2.auth_hash = hash;
    orderBook.addOrder(0, buyOrder1);
    orderBook.addOrder(1, buyOrder2);
    orderBook.addOrder(2, sellOrder1);
    orderBook.addOrder(3, sellOrder2);
    std::thread readerThread(&OrderReader::readOrder, &reader);
    std::thread matcherThread(&OrderBook::match, &orderBook);
    std::thread loggerThread(&OrderLogger::log, &logger);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    reader.stopReader();
    orderBook.stopMatch();
    logger.stopLogger();

    readerThread.join();
    matcherThread.join();
    loggerThread.join();
}

int main() {
    testOrderBook();
    return 0;
}