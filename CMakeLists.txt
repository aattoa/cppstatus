cmake_minimum_required(VERSION 3.24)
project(cppstatus CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE src/${PROJECT_NAME}.cpp
    PRIVATE src/configuration.hpp
    PRIVATE src/block.hpp
    PRIVATE src/callbacks.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra" "-Wpedantic")
