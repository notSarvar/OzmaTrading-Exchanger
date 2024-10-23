#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "include/order_generator.h"
#include "include/order_logger.h"
#include "include/order_reader.h"

int main(int argc, char **argv) {
  if (argc != 6) {
    std::cerr << "Usage: " << argv[0] << " <U> <N> <M> <min_price> <max_price>"
              << std::endl;
    return 1;
  }

  int U, N, M, min_price, max_price;
  std::istringstream(argv[1]) >> U;
  std::istringstream(argv[2]) >> N;
  std::istringstream(argv[3]) >> M;
  std::istringstream(argv[4]) >> min_price;
  std::istringstream(argv[5]) >> max_price;

  RingBuffer<Order> orderBuffer(U * M);
  RingBuffer<std::string> logBuffer(U * M);

  OrderLogger logger(logBuffer, "orders.log");
  OrderBook orderBook(logger);

  OrderGenerator generator(orderBuffer, min_price, max_price, N, U);
  OrderReader reader(orderBuffer, orderBook, min_price, max_price, N, M, U);

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