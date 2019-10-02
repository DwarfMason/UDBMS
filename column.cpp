#include "column.h"

column::column(std::string name, data_type type)
{
    set_name(name);
    set_type(type);
}
column::~column()
{

}
void column::set_name(std::string name)
{
    name_ = std::move(name);
}
void column::set_type(data_type type)
{
    type_ = type;
}
void column::set_size(uint32_t size)
{
    size_ = size;
}
void column::set_constraints(constraints cts)
{
    constraints_ = cts;
}
std::string column::get_name() const
{
    return name_;
}
const data_type &column::get_type() const
{
    return type_;
}
uint32_t column::get_size() const
{
    return size_;
}
const constraints &column::get_constraints() const
{
    return constraints_;
}

