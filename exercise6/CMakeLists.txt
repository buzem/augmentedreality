cmake_minimum_required(VERSION 3.10)
project(exercise6)

set(CMAKE_CXX_STANDARD 11)

add_executable(exercise6 main.cpp DrawPrimitives.h)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS})

target_link_libraries(exercise6 ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} glfw)