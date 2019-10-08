#pragma once

#include <fstream>
#include <memory>
#include <filesystem>
#include <api/data_type.h>

namespace fs = std::filesystem;

class table_data
{
public:
    table_data() = default;
    table_data(const std::string& name);
    std::shared_ptr<void> read_value(size_t offset, data_type type);
    void write_value(size_t offset, data_type type, const std::shared_ptr<void>& value);
private:
    fs::path storage_path_;
    void check_requirements_();
};
