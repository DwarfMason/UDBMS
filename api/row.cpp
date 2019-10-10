#include "row.h"
row::row(const std::vector<uint64_t> &sizes)
{
    sizes_ = sizes;
    for (auto size : sizes_)
    {
        offsets_.push_back(row_size);
        row_size += size;
    }
}
std::shared_ptr<void> row::at(size_t index)
{
    if (index < 0 and index >= sizes_.size())
    {
        throw std::runtime_error("Bad column index");
    }
    throw std::runtime_error("mda");
    //return std::make_shared<char>(reinterpret_cast<char*>(data_.get()) + offsets_[index]);
}
void row::set_data(std::shared_ptr<void> &&data)
{
    data_ = data;
}
