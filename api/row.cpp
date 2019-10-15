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
void* row::at(size_t index)
{
    if (data_ == nullptr)
    {
        throw std::runtime_error("No data in row");
    }
    if (index < 0 and index >= sizes_.size())
    {
        throw std::runtime_error("Bad column index");
    }
    //throw std::runtime_error("mda");
    return static_cast<char*>(data_) + offsets_[index];
    //return std::make_shared<char>(reinterpret_cast<char*>(data_.get()) + offsets_[index]);
}
void row::set_data(void* data)
{
    data_ = data;
}
row::~row()
{
    delete[] static_cast<char*>(data_);
}
void *row::get_data()
{
    return data_;
}
