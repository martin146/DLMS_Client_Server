project(gurux_dlms_cpp)

cmake_minimum_required(VERSION 3.9)

file(GLOB_RECURSE sources src/*.cpp)

add_library(gurux_dlms_cpp
  ${sources}
)

target_include_directories(gurux_dlms_cpp
  PUBLIC
    include
)

target_compile_definitions(gurux_dlms_cpp
    PRIVATE
        UNICODE)

target_compile_options(gurux_dlms_cpp
  PUBLIC
)