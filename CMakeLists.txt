cmake_minimum_required(VERSION 3.15)
project(UDBMS)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext/jsoncons/include)

FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)


BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/parser/commands.y ${CMAKE_CURRENT_BINARY_DIR}/commands.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/commands.h)

FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/parser/names.l ${CMAKE_CURRENT_BINARY_DIR}/names.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(UDBMS
        main.cpp
        storage_engine.cpp storage_engine.h
        table.cpp table.h
        column.cpp column.h
        constraints.cpp constraints.h
        data_type.cpp data_type.h
        exceptions.cpp exceptions.h
        ${FLEX_Lexer_OUTPUTS})

target_link_libraries(UDBMS INTERFACE stdc++fs)