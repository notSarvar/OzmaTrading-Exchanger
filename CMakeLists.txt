cmake_minimum_required(VERSION 3.21)
project(HWOzmaTrading)

set(CMAKE_CXX_STANDARD 17)
include_directories(.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

add_library(OzmaTradingLib STATIC ${SOURCES} ${HEADERS})

add_executable(HWOzmaTrading src/main.cpp)
add_executable(HWOzmaTradingTest tests/test.cpp)
add_executable(HWOzmaTradingAnalyze tests/analyze.cpp)

target_link_libraries(HWOzmaTrading OzmaTradingLib)
target_link_libraries(HWOzmaTradingTest OzmaTradingLib)
target_link_libraries(HWOzmaTradingAnalyze OzmaTradingLib)
