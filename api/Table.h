#pragma once

#include <string>
#include <vector>
#include <jsoncons/json.hpp>
#include <engine/table_data.h>
#include "Column.h"
#include "Row.h"

class Table
{
public:
    Table() = default;
    Table(const std::string &name);
    ~Table();
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] const std::vector<Column> &get_columns() const;
    void set_name(std::string name);
    void set_columns(std::vector<Column> cols);
    void add_column(const Column& col);
    [[nodiscard]] table_data& get_data();
    [[nodiscard]] std::vector<Row> get_rows();
    [[nodiscard]] std::string get_create_query() const;
    void load_data();
    void insert_row(const std::map<std::string, void*>& data);
    void delete_row(const std::string& col_name, const void* val); // EQUALS only
    void delete_row(std::vector<Row>::iterator pos);
    void set_cell_values(Row& r, const std::map<std::string, void*>& kv);
    std::vector<Row>::iterator find_first(const std::string& col_name, const void* val);
    std::vector<Row>::iterator find_next(const std::string& col_name, const void* val,
        std::vector<Row>::iterator start);
private:
    std::string name_;
    std::vector<Column> cols_;
    std::map<std::string, size_t> col_index;
    std::vector<uint64_t> sizes_;
    std::vector<Row> rows_;
    table_data data_;
};

JSONCONS_GETTER_SETTER_NAMED_TRAITS_DECL(Table,
                                         (get_name, set_name, "name"),
                                         (get_columns, set_columns, "columns"))