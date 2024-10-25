#include "include/order_logger.h"
#include "include/timer_loger.h"

void OrderLogger::ExportLog() {
  std::string log;
  if (log_buffer_.pop(log)) {
    static bool first_open_ = true;
    std::ofstream file(export_filename_, first_open_ ? std::ios::trunc : std::ios::app);
    first_open_ = false;
    file << log << std::endl;
  }
}

void OrderLogger::ExportLogUntil() {
  while (!stop_log_) {
    {
      TimeLogger logger("ExportLog", "TimeLog.txt");
      ExportLog();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void OrderLogger::LogMatched(const Order& buy_order, const Order& sell_order,
    int32_t size) {
    std::string log = "Matched {\n Buy order:\n\tPrice: " + std::to_string(buy_order.price) +
        "\n\tSize left: " + std::to_string(buy_order.size) +
        "\n\tHash: " + buy_order.auth_hash +
        "\n Sell order:\n\tPrice: " + std::to_string(sell_order.price) +
        "\n\tSize left: " + std::to_string(sell_order.size) +
        "\n\tHash: " + sell_order.auth_hash +
        "\n Amount: " + std::to_string(size) + "\n}";

    log_buffer_.push(log);
}


void OrderLogger::StopExport() { stop_log_ = true; }