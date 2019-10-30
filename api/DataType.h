#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <nlohmann/json.hpp>

struct type_info
{
    std::string name;
    uint64_t size;
};

enum class DataType: uint8_t
{
    INTEGER,
    FLOAT,
    CHAR,
};

#define REGISTER_TYPE(type, size) { DataType::type, { (#type) , (size) } }

extern std::map<DataType, type_info> type_registry;

NLOHMANN_JSON_SERIALIZE_ENUM(DataType, {
                             {DataType::INTEGER, "INTEGER"},
                             {DataType::FLOAT, "FLOAT"},
                             {DataType::CHAR, "CHAR"}
});

template <DataType SqlType>
struct cell_type {
    using value = void;
};

template <>
struct cell_type<DataType::INTEGER>
{
    using value = int;
};

template <>
struct cell_type<DataType::FLOAT>
{
    using value = float;
};

template <>
struct cell_type<DataType::CHAR>
{
    using value = std::string;
};

template <DataType SqlType>
using cell_type_v = typename cell_type<SqlType>::value;
