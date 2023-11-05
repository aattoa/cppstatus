cmake_minimum_required(VERSION 3.24)
project(cppstatus CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE src/main.cpp
    PRIVATE src/status.cpp
    PRIVATE src/status.hpp
    PRIVATE src/callbacks.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE X11)

target_compile_options(${PROJECT_NAME}
    PRIVATE "-Wall" "-Wextra" "-Wpedantic")
