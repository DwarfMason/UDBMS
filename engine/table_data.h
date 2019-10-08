#pragma once

#include <fstream>
#include <memory>
#include <api/data_type.h>
#include <api/table.h>

class table_data
{
public:
    table_data() = 0;
    table_data(const table& tbl);
    std::shared_ptr<void> read_value(size_t offset, data_type type);
    void write_value(size_t offset, data_type type, const std::shared_ptr<void>& value);
private:
    std::fstream data_file_;
    void check_requirements_();
};
