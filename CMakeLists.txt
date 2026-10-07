# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(BleKeyMapper CXX)

# Set the C++ standard to C++17 minimum
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target for the project
add_executable(BleKeyMapper src/main.cpp)