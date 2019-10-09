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

#define REGISTER_TYPE(type, size) { data_type::type, { #type , size } }

std::map<data_type, type_info> type_registry = {
    REGISTER_TYPE(INTEGER, 4),
    REGISTER_TYPE(FLOAT, 4),
    REGISTER_TYPE(CHAR, 1),
};

JSONCONS_ENUM_TRAITS_DECL(data_type, INTEGER, FLOAT, CHAR);