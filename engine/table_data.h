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
    void* read_some(size_t offset, uint64_t size);
    void* read_value(size_t offset, data_type type);
    void write_value(size_t offset, data_type type, void* value);
    void push_row(uint64_t row_size);
    void purge();
    [[nodiscard]] uint64_t get_row_count() const;
private:
    fs::path storage_path_;
    uint64_t row_count_ = 0;
    void check_requirements_();
    inline void update_row_count_(std::fstream& f);
};
