#include "table_data.h"

#include <filesystem>
#include <constants.h>

namespace fs = std::filesystem;

table_data::table_data(const table &tbl)
{
    std::string table_name = tbl.get_name();
    auto data_filename = table_name + STORAGE_EXT;
    fs::path storage_path(DATA_PATH / data_filename);

    // A table object exists, so should exist a data file
    // As a table object is created from meta file
    data_file_ = std::fstream(storage_path);
}

std::shared_ptr<void> table_data::read_value(size_t offset, data_type type)
{
    int64_t size = data_sizes.at(type);
    auto value = std::make_shared<void>(size);
    data_file_.seekg(offset, std::ios_base::beg);
    data_file_.readsome(std::dynamic_pointer_cast<char>(value).get(), size);
    return value;
}

void table_data::write_value(size_t offset, data_type type, const std::shared_ptr<void>& value)
{
    int64_t size = data_sizes.at(type);
    data_file_.seekp(offset, std::ios_base::beg);
    data_file_.write(std::dynamic_pointer_cast<char>(value).get(), size);
}
