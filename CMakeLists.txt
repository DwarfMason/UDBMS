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
        storage_engine.cpp storage_engine.h
        table.cpp table.h
        column.cpp column.h
        constraints.cpp constraints.h
        data_type.cpp data_type.h
        exceptions.cpp exceptions.h
        parser/driver.cpp
        ${FLEX_flexer_OUTPUTS}
        ${BISON_bison_parser_OUTPUTS}
        parser/statement/type_include.h parser/statement/BaseStatement.cpp parser/statement/BaseStatement.h parser/statement/CreateStatement.cpp parser/statement/CreateStatement.h)

target_link_libraries(UDBMS INTERFACE stdc++fs)