cmake_minimum_required(VERSION 3.10)
project(exercise2)

set(CMAKE_CXX_STANDARD 11)

add_executable(exercise2 main.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(exercise2 ${OpenCV_LIBS})