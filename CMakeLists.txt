cmake_minimum_required(VERSION 3.24)
project(ThreadsSynchronization)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-gdwarf-4)

add_executable(ThreadsSynchronization main.cpp include/threadArgs.h include/storeMonitor.h include/fileManager.h src/fileManager.cpp)
