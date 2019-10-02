#pragma once

#include <exception>
#include <stdexcept>

struct table_not_exist_error: public std::runtime_error
{
    table_not_exist_error()
        : runtime_error("Table does not exist")
    {}
};

struct table_exist_error: public std::runtime_error
{
    table_exist_error()
        : runtime_error("Table does already exist")
    {}
};

