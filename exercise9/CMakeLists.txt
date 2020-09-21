cmake_minimum_required(VERSION 3.10)
project(exercise9)

set(CMAKE_CXX_STANDARD 11)

add_executable(exercise9 main.cpp DrawPrimitives.h PoseEstimation MarkerTracker)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS})

target_link_libraries(exercise9 ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} glfw)