#pragma once

#include <string>
#include <api/TableMetadata.h>
#include <api/data_wrap/BaseDataWrapper.h>

namespace TableFileWorker
{
    TableMetadata create_table(const std::string &name, const std::vector<Column> &cols);
    TableMetadata load_table(const std::string& name);
    BaseDataWrapper* load_table_data(const TableMetadata& tbl);
    void drop_table(const TableMetadata& tbl);
};


