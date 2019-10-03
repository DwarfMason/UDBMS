#pragma once

#include <exception>
#include <stdexcept>

struct sql_error : public std::runtime_error
{
    sql_error(const char* msg)
        : runtime_error(msg)
    {}
};

struct table_not_exist_error: public sql_error
{
    table_not_exist_error()
        : sql_error("Table does not exist")
    {}
};

struct table_exist_error: public sql_error
{
    table_exist_error()
        : sql_error("Table does already exist")
    {}
};

