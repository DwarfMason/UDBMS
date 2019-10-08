#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include "api/table.h"
#include "exceptions.h"

class storage_engine
{
public:
    static table load_table(const std::string &name);
    static void save_table(const table &tbl);
    static table create_table(const std::string &name);
    static void delete_table(const std::string &name);
    static std::string show_create_table(const table &tbl);
};
