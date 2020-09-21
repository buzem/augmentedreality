cmake_minimum_required(VERSION 3.10)
project(exercise5)

set(CMAKE_CXX_STANDARD 11)

add_executable(exercise5 main.cpp PoseEstimation)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(exercise5 ${OpenCV_LIBS})