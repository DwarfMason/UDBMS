cmake_minimum_required(VERSION 3.15)
project(UDBMS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DECHO -fsanitize=address")
include_directories(ext/jsoncons/include)
include_directories(ext/fmt/include)

FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)


BISON_TARGET(bison_parser
        parser/parser.yy
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)
FLEX_TARGET(flexer
        parser/lexer.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
set(bison -g)
ADD_FLEX_BISON_DEPENDENCY(flexer bison_parser)

include_directories(
    UDBMS
    PRIVATE
        .
        ${CMAKE_CURRENT_BINARY_DIR})

add_executable(UDBMS
        ext/fmt/src/format.cc
        main.cpp
        api/Table.cpp api/Table.h
        api/Column.cpp api/Column.h
        api/Constraints.cpp api/Constraints.h
        api/data_type.cpp api/data_type.h
        exceptions.cpp exceptions.h
        parser/driver.cpp
        ${FLEX_flexer_OUTPUTS}
        ${BISON_bison_parser_OUTPUTS}
        api/crud.cpp api/crud.h
        constants.h
        engine/table_data.cpp engine/table_data.h
        parser/statement/BaseStatement.h
        parser/statement/CreateStatement.h
        parser/statement/ShowCreateStatement.h
        parser/statement/DropTableStatement.h
        api/Cursor.cpp api/Cursor.h
        parser/statement/InsertStatement.h
        parser/statement/UpdateStatement.h
        parser/statement/DeleteStatement.h
        parser/statement/SelectStatement.h
        api/Row.cpp api/Row.h
        logic/show_create.cpp logic/show_create.h parser/condition/Condition.cpp parser/condition/Condition.h Server.cpp Server.h)

target_link_libraries(UDBMS INTERFACE stdc++fs)
target_link_libraries(UDBMS PRIVATE pthread)

# Client
add_subdirectory(UDBMS_CLIENT)