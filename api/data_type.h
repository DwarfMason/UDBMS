#pragma once

#include <cstdint>
#include <jsoncons/json.hpp>

enum class data_type: uint8_t
{
    INTEGER,
    FLOAT,
    CHAR,
};

const std::map<data_type, uint64_t> data_sizes = {
    {data_type::INTEGER, 4},
    {data_type::FLOAT, 4},
    {data_type::CHAR, 1},
};

JSONCONS_ENUM_TRAITS_DECL(data_type, INTEGER, FLOAT, CHAR);