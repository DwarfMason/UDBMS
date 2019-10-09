#include "table_data.h"

#include <constants.h>

const auto FSTREAM_DATA_MODE = std::fstream::in | std::fstream::out | std::fstream::binary;

table_data::table_data(const std::string &name)
{
    auto data_filename = name + STORAGE_EXT;
    storage_path_ = fs::path(DATA_PATH / data_filename);
    if (!fs::exists(storage_path_)) {
        purge();
    }
}

std::shared_ptr<void> table_data::read_value(size_t offset, data_type type)
{
    std::fstream data_file_(storage_path_, FSTREAM_DATA_MODE);
    uint64_t size = type_registry.at(type).size;
    auto value = std::make_shared<char>(size);
    data_file_.seekg(offset, std::ios_base::beg);
    data_file_.readsome(value.get(), size);
    return value;
}

void table_data::write_value(size_t offset, data_type type, const std::shared_ptr<void>& value)
{
    std::fstream data_file_(storage_path_, FSTREAM_DATA_MODE);
    uint64_t size = type_registry.at(type).size;
    data_file_.seekp(offset, std::ios_base::beg);
    data_file_.write(static_cast<char*>(value.get()), size);
}

void table_data::check_requirements_()
{
    // TODO check for errors
}
void table_data::push_row(uint64_t row_size)
{
    std::fstream data_file_(storage_path_, FSTREAM_DATA_MODE);
    data_file_.seekp(0, std::ios_base::end);
    std::shared_ptr<char> nulled = std::make_shared<char>(row_size, 0);
    data_file_.write(nulled.get(), row_size);

    ++row_count_;
    update_row_count_(data_file_);
}
uint64_t table_data::get_row_count() const
{
    return row_count_;
}
void table_data::update_row_count_(std::fstream& f)
{
    f.seekp(0, std::ios_base::beg);
    f.write(reinterpret_cast<const char *>(&row_count_), sizeof(row_count_));
}
void table_data::purge()
{
    row_count_ = 0;
    std::fstream f(storage_path_, FSTREAM_DATA_MODE | std::fstream::trunc);
    update_row_count_(f);
}
