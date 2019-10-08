#pragma once

#include <string>
#include <vector>
#include <jsoncons/json.hpp>
#include <engine/table_data.h>
#include "column.h"

class table
{
public:
    table() = default;
    table(const table& tbl);
    explicit table(const std::string& name);
    ~table();
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] const std::vector<column>& get_columns() const;
    void set_name(std::string name);
    void set_columns(std::vector<column> cols);
private:
    std::string name_;
    std::vector<column> cols_;
    table_data data_;
};

JSONCONS_GETTER_SETTER_NAMED_TRAITS_DECL(table,
                                         (get_name, set_name, "name"),
                                         (get_columns, set_columns, "columns"))