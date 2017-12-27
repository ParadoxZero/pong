cmake_minimum_required(VERSION 3.8)
project(pong)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_C_STANDARD 99)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES src/main.c src/Circle.c src/Circle.h)
add_executable(pong ${SOURCE_FILES})
target_link_libraries(pong ${SDL2_LIBRARY})
