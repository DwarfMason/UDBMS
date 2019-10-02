#include "table.h"

#include <utility>

table::table(const std::string& name)
{
    set_name(name);
}

table::~table()
{

}

std::string table::get_name() const
{
    return name_;
}
void table::set_name(std::string name)
{
    name_ = std::move(name);
}

const std::vector<column>& table::get_columns() const
{
    return cols_;
}
void table::set_columns(const std::vector<column>& cols)
{
    cols_ = cols;
}