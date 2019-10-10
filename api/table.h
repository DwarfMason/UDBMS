#pragma once

#include <string>
#include <vector>
#include <jsoncons/json.hpp>
#include <engine/table_data.h>
#include "column.h"
#include "row.h"

class table
{
public:
    table() = default;
    table(const std::string &name);
    ~table();
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] const std::vector<column> &get_columns() const;
    void set_name(std::string name);
    void set_columns(std::vector<column> cols);
    void add_column(const column& col);
    [[nodiscard]] table_data& get_data();
    [[nodiscard]] std::vector<row>& get_rows();
    [[nodiscard]] std::string get_create_query() const;
    void load_data();
    void insert_row(const std::map<std::string, std::shared_ptr<void>>& data);
    void delete_row(const std::string& col_name, const std::shared_ptr<void>& val); // EQUALS only
    // std::vector<row>::iterator find_row(const std::string& col_name, const std::shared_ptr<void>& val, );
private:
    std::string name_;
    std::vector<column> cols_;
    std::map<std::string, size_t> col_index;
    std::vector<uint64_t> sizes_;
    std::vector<row> rows_;
    table_data data_;
    size_t find_row(const std::string& col_name, const std::shared_ptr<void>& val);
};

JSONCONS_GETTER_SETTER_NAMED_TRAITS_DECL(table,
                                         (get_name, set_name, "name"),
                                         (get_columns, set_columns, "columns"))