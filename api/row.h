#pragma once

#include <memory>
#include <vector>

class row
{
public:
    explicit row(const std::vector<uint64_t>& sizes);
    void set_data(std::shared_ptr<void>&& data);
    std::shared_ptr<void> at(size_t index);

private:
    std::shared_ptr<void> data_;
    std::vector<uint64_t> sizes_, offsets_;
    uint64_t row_size = 0;
};