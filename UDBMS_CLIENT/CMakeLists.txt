cmake_minimum_required(VERSION 3.15)
project(UDBMS_CLIENT)

set(CMAKE_CXX_STANDARD 17)

ADD_EXECUTABLE( UDBMS_CLIENT main.cpp tests.cpp Client.cpp Client.h)

find_package(GTest REQUIRED)
target_link_libraries(UDBMS_CLIENT ${GTEST_LIBRARIES})
