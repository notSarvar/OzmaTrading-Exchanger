cmake_minimum_required(VERSION 3.21)
project(HWOzmaTrading)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenSSL REQUIRED)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR} .)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

add_executable(HWOzmaTrading ${SOURCES} ${HEADERS})
