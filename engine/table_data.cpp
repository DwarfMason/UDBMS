#include "table_data.h"

#include <constants.h>

table_data::table_data(const std::string &name)
{
    auto data_filename = name + STORAGE_EXT;
    storage_path_ = fs::path(DATA_PATH / data_filename);
}

std::shared_ptr<void> table_data::read_value(size_t offset, data_type type)
{
    std::fstream data_file_(storage_path_);
    int64_t size = data_sizes.at(type);
    auto value = std::make_shared<char>(size);
    data_file_.seekg(offset, std::ios_base::beg);
    data_file_.readsome(value.get(), size);
    return value;
}

void table_data::write_value(size_t offset, data_type type, const std::shared_ptr<void>& value)
{
    std::fstream data_file_(storage_path_);
    int64_t size = data_sizes.at(type);
    data_file_.seekp(offset, std::ios_base::beg);
    data_file_.write(static_cast<char*>(value.get()), size);
}

void table_data::check_requirements_()
{
    // TODO check for errors
}
