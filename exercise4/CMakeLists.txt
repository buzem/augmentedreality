cmake_minimum_required(VERSION 3.10)
project(exercise4)

set(CMAKE_CXX_STANDARD 11)

add_executable(exercise4 main.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(exercise4 ${OpenCV_LIBS})