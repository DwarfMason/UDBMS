#pragma once

#include <cstdint>
#include <string>
#include <jsoncons/json.hpp>

struct type_info
{
    std::string name;
    uint64_t size;
};

enum class data_type: uint8_t
{
    INTEGER,
    FLOAT,
    CHAR,
};

#define REGISTER_TYPE(type, size) { data_type::type, { (#type) , (size) } }

extern std::map<data_type, type_info> type_registry;

JSONCONS_ENUM_TRAITS_DECL(data_type, INTEGER, FLOAT, CHAR);

template <data_type SqlType>
struct cell_type {};

template <>
struct cell_type<data_type::INTEGER>
{
    using value = int;
};

template <>
struct cell_type<data_type::FLOAT>
{
    using value = float;
};

template <>
struct cell_type<data_type::CHAR>
{
    using value = std::string;
};