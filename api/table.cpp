#include "table.h"

#include <utility>
#include <fmt/format.h>

table::table(const std::string& name)
{
    set_name(name);
    data_ = table_data(name);
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

void table::add_column(const column &col)
{
    cols_.push_back(col);
}
table_data &table::get_data()
{
    return data_;
}

std::string table::get_create_query() const
{
    std::stringstream create_args, constraint_args;
    const std::string sql_query_tpl = "CREATE TABLE `{}` (\n{});";
    const std::string arg_tpl = "  `{}` {}{}\n";
    for (int i = 0; i < cols_.size(); ++i)
    {
        const column& col = cols_[i];
        const constraints& cts = col.get_constraints();
        if (cts.is_primary_key()) {
            constraint_args << " PRIMARY KEY";
        } else {
            if (cts.is_not_null()) {
                constraint_args << " NOT NULL";
            }
            if (cts.is_unique_key()) {
                constraint_args << " UNIQUE";
            }
        }
        if (cts.is_index()) {
            constraint_args << " INDEX";
        }
        if (i < cols_.size()-1)
            constraint_args << ",";

        create_args << fmt::format(arg_tpl,
            col.get_name(),
            type_registry.at(col.get_type()).name,
            constraint_args.str()
        );
        constraint_args.clear();
    }
    return fmt::format(sql_query_tpl,
        name_,
        create_args.str()
    );
}
