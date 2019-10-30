cmake_minimum_required(VERSION 3.15)
project(UDBMS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DECHO -fsanitize=address")
#libfort
include_directories(ext/libfort/lib)
# jsoncons
include_directories(ext/jsoncons/include)
# nlohmann json
include_directories(ext/nlohmann_json)
# fmt
include_directories(ext/fmt/include)
# Storage engine
add_subdirectory(ext/YeltsinDB)
include_directories(ext/YeltsinDB/inc)

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
        ext/nlohmann_json/nlohmann/json.hpp
        ext/fmt/src/format.cc
        ext/libfort/lib/fort.hpp
        ext/libfort/lib/fort.c
        ext/libfort/lib/fort.h
        main.cpp
        exceptions.cpp exceptions.h
        parser/driver.cpp
        ${FLEX_flexer_OUTPUTS}
        ${BISON_bison_parser_OUTPUTS}
        constants.h
        parser/statement/BaseStatement.h
        parser/statement/CreateStatement.h
        parser/statement/ShowCreateStatement.h
        parser/statement/DropTableStatement.h
        parser/statement/InsertStatement.h
        parser/statement/UpdateStatement.h
        parser/statement/DeleteStatement.h
        parser/statement/SelectStatement.h
        logic/show_create.cpp logic/show_create.h
        parser/condition/Condition.cpp parser/condition/Condition.h
        Server.cpp Server.h

        api/TableMetadata.cpp api/TableMetadata.h
        api/Column.cpp api/Column.h
        api/Constraints.cpp api/Constraints.h
        api/Row.cpp api/Row.h
        api/Cell.cpp api/Cell.h
        api/data_wrap/BaseDataWrapper.cpp api/data_wrap/BaseDataWrapper.h
        api/data_wrap/YDBDataWrapper.cpp api/data_wrap/YDBDataWrapper.h
        api/TableFileWorker.cpp api/TableFileWorker.h
        api/DataType.cpp api/DataType.h)

target_link_libraries(UDBMS INTERFACE stdc++fs)
target_link_libraries(UDBMS PRIVATE pthread)
target_link_libraries(UDBMS PUBLIC YeltsinDB)

# Client
add_subdirectory(UDBMS_CLIENT)