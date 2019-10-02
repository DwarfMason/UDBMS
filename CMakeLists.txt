cmake_minimum_required(VERSION 3.15)
project(UDBMS)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext/jsoncons/include)

add_executable(UDBMS
        main.cpp
        storage_engine.cpp storage_engine.h
        table.cpp table.h
        column.cpp column.h
        constraints.cpp constraints.h
        data_type.cpp data_type.h
        exceptions.cpp exceptions.h)

target_link_libraries(UDBMS INTERFACE stdc++fs)