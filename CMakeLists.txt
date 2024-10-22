cmake_minimum_required(VERSION 3.21)
project(HWOzmaTrading)

set(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(Boost 1.40 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} .)

add_executable(HWOzmaTrading src/main.cpp)