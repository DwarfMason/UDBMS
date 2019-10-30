#include "Column.h"

#include <utility>

using json = nlohmann::json;

Column::Column(std::string name, DataType type)
{
    set_name(std::move(name));
    set_type(type);
}
Column::~Column()
{

}
void Column::set_name(std::string name)
{
    name_ = std::move(name);
}
void Column::set_type(DataType type)
{
    type_ = type;
}
void Column::set_size(uint32_t size)
{
    size_ = size;
}
void Column::set_constraints(Constraints cts)
{
    constraints_ = cts;
}
std::string Column::get_name() const
{
    return name_;
}
const DataType &Column::get_type() const
{
    return type_;
}
uint32_t Column::get_size() const
{
    return size_;
}
const Constraints &Column::get_constraints() const
{
    return constraints_;
}

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
