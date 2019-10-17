#include <fmt/format.h>
#include "show_create.h"

void Logic::show_create_table(const Table &tbl)
{
    const std::string output_tpl = "*************************** 1. row ***************************\n"
                                   "       Table: {}\n"
                                   "Create Table: {}\n";
    std::cout << fmt::format(output_tpl, tbl.get_name(), tbl.get_create_query());
}