#include "table.h"

#include <utility>

table::table(const std::string& name)
{
    set_name(name);
}

table::table(const table &tbl)
{
    // We need this because table_data has a deleted constructor
    set_name(tbl.name_);
    set_columns(tbl.cols_);
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
void table::set_columns(std::vector<column> cols)
{
    cols_ = std::move(cols);
}