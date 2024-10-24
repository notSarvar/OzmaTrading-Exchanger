cmake_minimum_required(VERSION 3.21)
project(HWOzmaTrading)

set(CMAKE_CXX_STANDARD 17)
include_directories(.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

add_executable(HWOzmaTrading ${SOURCES} ${HEADERS})

add_executable(HWOzmaTradingTest tests/test.cpp ${HEADERS}
    src/order_generator.cpp
    src/order_book.cpp
    src/hash_order.cpp
    src/order_logger.cpp
    src/order_reader.cpp
)