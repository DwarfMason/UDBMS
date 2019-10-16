#pragma once

#include <exception>
#include <stdexcept>
#include <sstream>

struct sql_error : public std::runtime_error
{
    explicit sql_error(uint16_t errcode, const std::string& msg)
        : runtime_error(msg)
    {
        error_code_ = errcode;
        msg_ = msg;
    }
    [[nodiscard]] std::string msg() const noexcept
    {
        std::stringstream ss;
        ss << "Error " << std::to_string(error_code_) << ": " << msg_;
        const auto tmp = ss.str();
        return tmp;
    }
    uint16_t error_code_ = 0;
    std::string msg_;
};

struct custom_exception : public sql_error
{
    custom_exception(uint16_t errcode, const std::string &msg)
        : sql_error(errcode,msg){
    }
};

struct table_not_exist_error: public sql_error
{
    table_not_exist_error()
        : sql_error(100, "Table does not exist")
    {}
};

struct parser_error: public sql_error
{
    parser_error(std::string err)
        : sql_error(200, err)
    {}
};

struct table_exist_error: public sql_error
{
    table_exist_error()
        : sql_error(101, "Table does already exist")
    {}
};

struct select_no_matches_error: public sql_error
{
    select_no_matches_error()
        : sql_error(300, "No matches found")
    {}
};

struct cursor_eof_error: public sql_error
{
    cursor_eof_error()
        : sql_error(301, "Cursor has reached the end")
    {}
};
