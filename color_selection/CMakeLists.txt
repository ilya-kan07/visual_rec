cmake_minimum_required(VERSION 3.10)
project(MyProject)

find_package(OpenCV REQUIRED)

add_executable(my_prog main.cpp)
target_link_libraries(my_prog ${OpenCV_LIBS})
include_directories(${OpenCV_INCLUDE_DIRS})
