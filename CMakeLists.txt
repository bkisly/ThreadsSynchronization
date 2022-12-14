cmake_minimum_required(VERSION 3.24)
project(ThreadsSynchronization)

set(CMAKE_CXX_STANDARD 20)

add_executable(ThreadsSynchronization main.cpp include/amountInfo.h include/storeMonitor.h src/storeMonitor.cpp include/fileManager.h src/fileManager.cpp)
