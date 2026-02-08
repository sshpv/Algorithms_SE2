cmake_minimum_required(VERSION 3.16)
project(hyperloglog_study CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hll
    src/main.cpp
    src/random_stream_gen.cpp
    src/hash_func_gen.cpp
    src/hyperloglog.cpp
)

target_include_directories(hll PRIVATE src)
