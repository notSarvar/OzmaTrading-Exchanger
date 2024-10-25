#include "include/order_generator.h"
#include "include/order_logger.h"
#include "include/order_reader.h"
#include "include/timer_loger.h"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char **argv) {
  if (argc != 6) {
    std::cerr << "Usage: " << argv[0] << " <U> <N> <M> <min_price> <max_price>"
              << std::endl;
    return 1;
  }

  int32_t U, N, M, min_price, max_price;
  std::istringstream(argv[1]) >> U;
  std::istringstream(argv[2]) >> N;
  std::istringstream(argv[3]) >> M;
  std::istringstream(argv[4]) >> min_price;
  std::istringstream(argv[5]) >> max_price;

  RingBuffer<Order> orderBuffer(U * M);
  RingBuffer<std::string> logBuffer(U * M);

  OrderLogger logger(logBuffer, "orders.log");
  OrderBook orderBook(logger, min_price, max_price, N, M, U);

  OrderGenerator generator(orderBuffer, min_price, max_price, N, U);
  OrderReader reader(orderBuffer, orderBook);

  std::thread generatorThread(&OrderGenerator::GenerateUntil, &generator);
  std::thread readerThread(&OrderReader::Read, &reader);
  std::thread matcherThread(&OrderBook::MatchUntil, &orderBook);
  std::thread loggerThread(&OrderLogger::ExportLogUntil, &logger);

  std::this_thread::sleep_for(std::chrono::seconds(10));

  generator.StopGenerator();
  reader.StopReader();
  orderBook.StopMatch();
  logger.StopExport();

  generatorThread.join();
  readerThread.join();
  matcherThread.join();
  loggerThread.join();

  ExportTimelogs("TimeLog.txt");

  return 0;
}