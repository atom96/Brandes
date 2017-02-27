cmake_minimum_required(VERSION 3.1)
project(brandes)
find_package ( Threads REQUIRED )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(brandes ${SOURCE_FILES})
install(TARGETS DESTINATION .)