#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include "api/table.h"
#include "exceptions.h"
#include "table_data.h"

namespace storage_engine
{
    table load_table(const std::string &name);
    void save_table(const table &tbl);
    table create_table(const std::string &name);
    void delete_table(const std::string &name);
    std::string show_create_table(const table &tbl);

    table_data save_data(const table_data& data);
    table_data load_data(const std::string& name);
};
