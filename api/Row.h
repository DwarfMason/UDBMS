#pragma once

#include <memory>
#include <vector>

class Row
{
public:
    explicit Row(const std::vector<uint64_t>& sizes);
    ~Row();
    void* get_data();
    void set_data(void* data);
    void* at(size_t index);

private:
    void* data_ = nullptr;
    std::vector<uint64_t> sizes_, offsets_;
    uint64_t row_size = 0;
};