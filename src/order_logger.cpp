#include "include/order_logger.h"

void OrderLogger::ExportLog() {
  std::string log;
  if (log_buffer_.pop(log)) {
    std::ofstream file(export_filename_, std::ios::app);
    file << log << std::endl;
  }
}

void OrderLogger::ExportLogUntil() {
  while (!stop_log_) {
    ExportLog();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderLogger::LogMatched(const Order &buy_order, const Order &sell_order,
                           int32_t size) {
  std::ostringstream log_stream;
  log_stream << "Matched {\n Buy order:\n\tPrice: " << buy_order.price 
             << "\n\tSize left: " << buy_order.size 
             << "\n\tHash: " << buy_order.auth_hash 
             << "\n Sell order:\n\tPrice: " << sell_order.price 
             << "\n\tSize left: " << sell_order.size 
             << "\n\tHash: " << sell_order.auth_hash 
             << " \n Amount: " << size << "\n}";
  std::string log = log_stream.str();
  log_buffer_.push(log);
}

void OrderLogger::StopExport() { stop_log_ = true; }