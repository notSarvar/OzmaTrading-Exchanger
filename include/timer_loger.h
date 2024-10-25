#pragma once

#include <fstream>

class TimeLogger{
public:
    TimeLogger(const std::string& stage, const std::string& export_filename_) : stage_(stage), export_filename_(export_filename_) {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~TimeLogger() {
        auto end = std::chrono::high_resolution_clock::now();
        std::ofstream file(export_filename_, std::ios::app);
        std::chrono::duration<double> duration = end - start_;
        std::string result = stage_ + ": " + std::to_string(duration.count()) + " seconds\n";
        file << result;
    }
private:
    std::string stage_;
    std::string export_filename_;    
    std::chrono::high_resolution_clock::time_point start_;  
}; 