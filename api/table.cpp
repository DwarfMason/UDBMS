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
    sizes_.clear();
    col_index.clear();
    for (size_t i = 0; i < cols_.size(); ++i)
    {
        const column& col = cols_[i];
        sizes_.push_back(col.get_size());
        col_index.insert_or_assign(col.get_name(), i);
    }
}

void table::add_column(const column &col)
{
    cols_.push_back(col);
    col_index.insert_or_assign(col.get_name(), sizes_.size());
    sizes_.push_back(col.get_size());
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

void table::insert_row(const std::map<std::string, std::shared_ptr<void>>& data)
{
    auto& row = rows_.emplace_back(sizes_);
    uint64_t row_size = 0;
    for (uint64_t s : sizes_)
    {
        row_size += s;
    }
    row.set_data(std::make_shared<char>(new char[row_size], std::default_delete<char[]>()));
    for (const auto& kv : data) {
        auto cell = row.at(col_index[kv.first]);
        memcpy(cell.get(), kv.second.get(),
            type_registry.at(cols_[col_index[kv.first]].get_type()).size);
    }
}
void table::delete_row(const std::string &col_name, const std::shared_ptr<void>& val)
{
    size_t row_i = find_row(col_name, val);
    rows_.erase(rows_.begin() + row_i);
}
size_t table::find_row(const std::string &col_name, const std::shared_ptr<void>& val)
{
    size_t col_i = col_index[col_name];
    uint64_t col_s = type_registry.at(cols_[col_i].get_type()).size;
    for (size_t i = 0; i < rows_.size(); ++i)
    {
        if (!memcmp(rows_[i].at(col_i).get(), val.get(), col_s))
        {
            return i;
        }
    }
    throw std::runtime_error("TODO: exception");
}
std::vector<row> &table::get_rows()
{
    return rows_;
}

void table::load_data()
{
    uint64_t row_cnt = data_.get_row_count();
    uint64_t offset = sizeof(row_cnt);
    uint64_t row_size = 0;
    for (uint64_t s : sizes_)
    {
        row_size += s;
    }

    for (uint64_t curr_row = 0; curr_row < row_cnt; ++row_cnt)
    {
        auto& x = rows_.emplace_back(sizes_);
        x.set_data(data_.read_some(offset += row_size, row_size));
    }
}