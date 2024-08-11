cmake_minimum_required(VERSION 3.1...3.29)

project( StringView VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O2")

include_directories(../src)

add_executable(test_string_view test_string_view.cc)

