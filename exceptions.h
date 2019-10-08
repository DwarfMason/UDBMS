#pragma once

#include <exception>
#include <stdexcept>

struct sql_error : public std::runtime_error
{
    explicit sql_error(uint16_t errcode, const std::string& msg)
        : runtime_error(msg)
    {
        error_code_ = errcode;
        msg_ = msg;
    }
    virtual const char* what() const noexcept
    {
        return ("Error " + std::to_string(error_code_) + ": " + msg_).c_str();
    }
    uint16_t error_code_ = 0;
    std::string msg_;
};

struct table_not_exist_error: public sql_error
{
    table_not_exist_error()
        : sql_error(100, "Table does not exist")
    {}
};

struct table_exist_error: public sql_error
{
    table_exist_error()
        : sql_error(101, "Table does already exist")
    {}
};

