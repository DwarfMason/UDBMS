#pragma once

#include <string>
#include <table.h>

namespace API
{
    table create_table(const std::string& name);
    table load_table(const std::string& name);
    void drop_table(const table& tbl);
    void commit_table(const table& tbl);
}