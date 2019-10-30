#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Constraints.h"
#include "DataType.h"

class Column
{
public:
    Column() = default;
    explicit Column(std::string name, DataType type);
    ~Column();
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] const DataType& get_type() const;
    [[nodiscard]] uint32_t get_size() const;
    [[nodiscard]] const Constraints& get_constraints() const;
    void set_name(std::string name);
    void set_type(DataType type);
    void set_size(uint32_t size);
    void set_constraints(Constraints cts);
private:
    std::string name_;
    DataType type_;
    uint32_t size_ = 0;
    Constraints constraints_;
};

void to_json(json& j, const Column& c) {
    j = json{
        {"name", c.get_name()},
        {"type", c.get_type()},
        {"size", c.get_size()},
        {"constraints", c.get_constraints()},
    };
}

void from_json(const json& j, Column& c) {
    c.set_name(j.at("name").get<std::string>());
    c.set_type(j.at("type").get<DataType>());
    c.set_size(j.at("size").get<uint32_t>());
    c.set_constraints(j.at("constraints").get<Constraints>());
}
