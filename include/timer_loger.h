#pragma once

#include "ring_buffer.hpp"

#include <fstream>
#include <iostream>

extern RingBuffer<std::string> Timelogs;

class TimeLogger{
public:
    TimeLogger(const std::string& stage, const std::string& export_filename_) : stage_(stage) {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~TimeLogger() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start_;
        std::string result = stage_ + ": " + std::to_string(duration.count()) + " seconds\n";
        Timelogs.push(result);
    }
private:
    std::string stage_;
    std::chrono::high_resolution_clock::time_point start_;  
}; 

void ExportTimelogs(const std::string& filename);