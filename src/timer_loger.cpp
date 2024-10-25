#include "include/timer_loger.h"

RingBuffer<std::string> Timelogs(1e6);

void ExportTimelogs(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        while (!Timelogs.empty()) {  
            std::string log;
            Timelogs.pop(log); 
            file << log; 
        }
        file.close();
    }
}