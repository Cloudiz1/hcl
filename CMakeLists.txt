cmake_minimum_required(VERSION 3.15)
project(hcl C)

add_library(hcl INTERFACE)
target_include_directories(hcl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
