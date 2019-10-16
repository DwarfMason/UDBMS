#include "table_data.h"

#include <constants.h>

const auto FSTREAM_DATA_MODE = std::fstream::in | std::fstream::out | std::fstream::binary;

table_data::table_data(const std::string &name)
{
    auto data_filename = name + STORAGE_EXT;
    storage_path_ = fs::path(DATA_PATH / data_filename);
    if (!fs::exists(storage_path_)) {
        purge();
    } else {
        auto a = read_some(0, 8);
        row_count_ = *static_cast<uint64_t*>(a);
    }
}

void* table_data::read_value(size_t offset, data_type type)
{
    return read_some(offset, type_registry.at(type).size);
}

void table_data::write_value(size_t offset, data_type type, void* value)
{
    write_some(offset, type_registry.at(type).size, value);
}

void table_data::check_requirements_()
{
    // TODO check for errors
}
void table_data::push_row(uint64_t row_size)
{
    std::fstream data_file_(storage_path_, FSTREAM_DATA_MODE);
    data_file_.seekp(0, std::fstream::end);
    auto iiii = data_file_.tellp();
    char* nulled = new char[row_size]();
    data_file_.write(nulled, row_size);

    ++row_count_;
    update_row_count_(data_file_);
    data_file_.close();
}
uint64_t table_data::get_row_count() const
{
    return row_count_;
}
void table_data::update_row_count_(std::fstream& f)
{
    f.seekp(0, std::fstream::beg);
    f.write(reinterpret_cast<const char *>(&row_count_), sizeof(row_count_));
}
void table_data::purge()
{
    row_count_ = 0;
    std::fstream f(storage_path_, FSTREAM_DATA_MODE | std::fstream::trunc);
    update_row_count_(f);
    f.close();
}
void* table_data::read_some(size_t offset, uint64_t size)
{
    std::fstream data_file_(storage_path_, FSTREAM_DATA_MODE);
    auto value = new char[size];
    data_file_.seekg(offset, std::fstream::beg);
    data_file_.readsome(value, size);
    data_file_.close();
    return value;
}
void table_data::write_some(size_t offset, uint64_t size, void* value)
{
    std::fstream data_file_(storage_path_, FSTREAM_DATA_MODE);
    data_file_.seekp(offset, std::fstream::beg);
    data_file_.write(static_cast<char*>(value), size);
    data_file_.close();
}
