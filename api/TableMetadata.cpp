#include "TableMetadata.h"

#include <utility>
#include <sstream>
#include <fmt/format.h>

TableMetadata::TableMetadata(std::string name, std::vector<Column> columns)
    : name_(std::move(name)), cols_(std::move(columns))
{
    for (size_t i = 0; i < cols_.size(); ++i) {
        col_index_.insert_or_assign(cols_[i].get_name(), i);
    }
}
std::string TableMetadata::get_name() const
{
    return name_;
}
void TableMetadata::set_name(const std::string &new_name)
{
    name_ = new_name;
}
std::vector<Column> TableMetadata::get_columns() const
{
    return cols_;
}
size_t TableMetadata::get_index_by_name(const std::string &col_name) const
{
    return col_index_.at(col_name);
}

std::string TableMetadata::get_create_query() const
{
    std::stringstream create_args, constraint_args;
    const std::string sql_query_tpl = "CREATE TABLE `{}` (\n{});";
    const std::string arg_tpl = "  `{}` {}{}\n";
    for (size_t i = 0; i < cols_.size(); ++i)
    {
        const Column& col = cols_[i];
        const Constraints& cts = col.get_constraints();
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

        std::string cts_str = constraint_args.str();

        std::string type_str = type_registry.at(col.get_type()).name;
        if (col.get_size() > 0)
        {
            type_str += "(" + std::to_string(col.get_size()) + ")";
        }

        create_args << fmt::format(arg_tpl,
            col.get_name(),
            type_str,
            cts_str
        );
        constraint_args.str("");
    }
    return fmt::format(sql_query_tpl,
        name_,
        create_args.str()
    );
}
