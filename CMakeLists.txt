cmake_minimum_required(VERSION 3.15)
project(UDBMS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DECHO")
include_directories(ext/jsoncons/include)

FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)


BISON_TARGET(bison_parser
        parser/parser.yy
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)
FLEX_TARGET(flexer
        parser/lexer.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(flexer bison_parser)

include_directories(
    UDBMS
    PRIVATE
        .
        ${CMAKE_CURRENT_BINARY_DIR})

add_executable(UDBMS
        main.cpp
        engine/storage_engine.cpp engine/storage_engine.h
        api/table.cpp api/table.h
        api/column.cpp api/column.h
        api/constraints.cpp api/constraints.h
        api/data_type.cpp api/data_type.h
        exceptions.cpp exceptions.h
        parser/driver.cpp
        ${FLEX_flexer_OUTPUTS}
        ${BISON_bison_parser_OUTPUTS}
        api/crud.cpp api/crud.h
        constants.h engine/table_data.cpp engine/table_data.h)

target_link_libraries(UDBMS INTERFACE stdc++fs)