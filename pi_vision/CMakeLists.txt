cmake_minimum_required(VERSION 3.10)
project(pi_vision)


set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

add_executable(live_demo src/live_demo.cpp)
target_link_libraries(live_demo ${OpenCV_LIBS})
add_executable(color_track src/color_track.cpp)
target_link_libraries(color_track ${OpenCV_LIBS})