#include "Column.h"

Column::Column(std::string name, data_type type)
{
    set_name(name);
    set_type(type);
}
Column::~Column()
{

}
void Column::set_name(std::string name)
{
    name_ = std::move(name);
}
void Column::set_type(data_type type)
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
const data_type &Column::get_type() const
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

