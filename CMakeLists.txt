cmake_minimum_required(VERSION 3.15)
project(UDBMS)

set(CMAKE_CXX_STANDARD 17)

add_executable(UDBMS main.cpp)

target_link_libraries(UDBMS stdc++fs)