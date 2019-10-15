#pragma once

#include <string>
#include <api/Table.h>

namespace API
{
    Table create_table(const std::string& name);
    Table load_table(const std::string& name);
    void drop_table(const Table& tbl);
    void commit_table(Table& tbl);
}