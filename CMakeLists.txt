cmake_minimum_required(VERSION 3.0)
project(fence_markup)
find_package(OpenCV REQUIRED)

add_executable(fence_markup main.cpp projections.hpp)
message(${OpenCV_LIBS})
target_link_libraries(fence_markup ${OpenCV_LIBS})

