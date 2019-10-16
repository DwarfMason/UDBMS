#include "Table.h"

#include <utility>
#include <fmt/format.h>
#include <exceptions.h>

Table::Table(const std::string& name)
{
    set_name(name);
    data_ = table_data(name);
}

Table::~Table()
{

}

std::string Table::get_name() const
{
    return name_;
}
void Table::set_name(std::string name)
{
    name_ = std::move(name);
    data_ = table_data(name_);
    // FIXME kavo?
}

const std::vector<Column>& Table::get_columns() const
{
    return cols_;
}
void Table::set_columns(std::vector<Column> cols)
{
    cols_ = std::move(cols);
    sizes_.clear();
    col_index.clear();
    for (size_t i = 0; i < cols_.size(); ++i)
    {
        const Column& col = cols_[i];
        auto sz = col.get_size();
        // TODO treat numeric types' size correct
        // TODO split physical and logical size
        if (sz == 0)
            sz = type_registry.at(col.get_type()).size;
        sizes_.push_back(sz);
        col_index.insert_or_assign(col.get_name(), i);
    }
}

void Table::add_column(const Column &col)
{
    cols_.push_back(col);
    col_index.insert_or_assign(col.get_name(), sizes_.size());
    sizes_.push_back(col.get_size());
}
table_data &Table::get_data()
{
    return data_;
}

std::string Table::get_create_query() const
{
    std::stringstream create_args, constraint_args;
    const std::string sql_query_tpl = "CREATE TABLE `{}` (\n{});";
    const std::string arg_tpl = "  `{}` {}{}\n";
    for (int i = 0; i < cols_.size(); ++i)
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

void Table::insert_row(const std::map<std::string, void*>& data)
{
    auto& row = rows_.emplace_back(sizes_);
    uint64_t row_size = 0;
    for (uint64_t s : sizes_)
    {
        row_size += s;
    }
    row.set_data(::operator new(row_size));
    for (const auto& kv : data) {
        auto cell = row.at(col_index[kv.first]);
        memcpy(cell, kv.second,
            type_registry.at(cols_[col_index[kv.first]].get_type()).size);
    }
}
void Table::delete_row(const std::string &col_name, const void* val)
{
    auto row = find_first(col_name, val);
    rows_.erase(row);
}

std::vector<Row> Table::get_rows()
{
    return rows_;
}

void Table::load_data()
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
std::vector<Row>::iterator Table::find_first(const std::string &col_name, const void* val)
{
    return find_next(col_name, val, rows_.begin());
}

std::vector<Row>::iterator Table::find_next(const std::string &col_name, const void* val,
                                            std::vector<Row>::iterator start)
{
    size_t col_i = col_index[col_name];
    uint64_t col_s = type_registry.at(cols_[col_i].get_type()).size;

    for (auto it = start; it != rows_.end(); ++it)
    {
        if (!memcmp(it->at(col_i), val, col_s))
        {
            return it;
        }
    }
    throw select_no_matches_error();
}
void Table::set_cell_values(Row& r, const std::map<std::string, void*>& kv)
{
    uint64_t row_size = 0;
    for (uint64_t s : sizes_)
    {
        row_size += s;
    }
    for (const auto& elem : kv) {
        auto cell = r.at(col_index[elem.first]);
        memcpy(cell, elem.second,
            type_registry.at(cols_[col_index[elem.first]].get_type()).size);
    }
}
void Table::delete_row(std::vector<Row>::iterator pos)
{
    rows_.erase(pos);
}
