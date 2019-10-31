#include <exceptions.h>
#include "Cell.h"

Cell::Cell(DataType type, uint32_t size)
{
    type_ = type;
    size_ = size;
    switch (type_)
    {
        case DataType::INTEGER:
            set_value(0);
            break;
        case DataType::FLOAT:
            set_value(0.0f);
            break;
        case DataType::CHAR:
            set_value("");
            break;
    }
}

Cell::Cell(DataType type, uint32_t size, const std::any& value) :
    Cell(type, size)
{
    set_value(value);
}

#define THROW_ON_MISMATCH(val, sql_type) static_assert(!std::is_void_v<cell_type_v<DataType::sql_type>>,\
    "No type defined for " #sql_type);\
static_assert(std::is_same_v<std::remove_cv_t<typeof(val)>, std::any>, "Value is not std::any");\
if (val.type() != typeid(cell_type_v<DataType::sql_type>)) { \
    throw api_wrong_assign_error(val.type().name(), typeid(cell_type_v<DataType::sql_type>).name());\
}


void Cell::set_value(const std::any& value)
{
    switch (type_)
    {
        case DataType::INTEGER:
            THROW_ON_MISMATCH(value, INTEGER);
            break;
        case DataType::FLOAT:
            THROW_ON_MISMATCH(value, FLOAT);
            break;
        case DataType::CHAR:
            THROW_ON_MISMATCH(value, CHAR);
            break;
    }
    value_ = value;
}

const void * Cell::to_raw() const
{
    switch (type_)
    {
        case DataType::INTEGER:
            return std::any_cast<cell_type_v<DataType::INTEGER>>(&value_);
        case DataType::FLOAT:
            return std::any_cast<cell_type_v<DataType::FLOAT>>(&value_);
        case DataType::CHAR:
            return std::any_cast<cell_type_v<DataType::CHAR>>(value_).c_str();
    }
    //return std::any_cast<void>(&value_);
}
void Cell::from_raw(const void *data)
{
    char d[size_ + 1];
    memcpy(d, data, size_);
    switch (type_)
    {
        case DataType::INTEGER:
            value_ = *reinterpret_cast<cell_type_v<DataType::INTEGER>*>(d);
            break;
        case DataType::FLOAT:
            value_ = *reinterpret_cast<cell_type_v<DataType::FLOAT>*>(d);
            break;
        case DataType::CHAR:
            value_ = std::string(d);
            break;
        default:
            throw std::runtime_error("from_raw() failed");
    }
}

uint32_t Cell::get_size() const
{
    return size_;
}
void Cell::set_size(uint32_t new_size)
{
    size_ = new_size;
}
