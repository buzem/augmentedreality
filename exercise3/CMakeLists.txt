cmake_minimum_required(VERSION 3.10)
project(exercise3)

set(CMAKE_CXX_STANDARD 11)

add_executable(exercise3 main.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(exercise3 ${OpenCV_LIBS})