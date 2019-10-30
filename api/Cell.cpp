#include <exceptions.h>
#include "Cell.h"

Cell::Cell(DataType type, uint32_t size)
{
    type_ = type;
    size_ = size;
}

Cell::Cell(DataType type, uint32_t size, const std::any& value) :
    Cell(type, size)
{
    set_value(value);
}

#define THROW_ON_MISMATCH(val, sql_type) static_assert(!std::is_void_v<cell_type_v<DataType::sql_type>>,\
    "No type defined for " #sql_type);\
static_assert(std::is_same_v<std::remove_cv_t<typeof(val)>, std::any>, "Value is not std::any");\
if (val.type() != typeid(cell_type_v<DataType::INTEGER>)) { \
    throw api_wrong_assign_error(val.type().name(), typeid(cell_type_v<DataType::sql_type>).name());\
}

#define THROW_IF_NOT_SQL_TYPE(val) THROW_ON_MISMATCH(val, INTEGER)\
THROW_ON_MISMATCH(val, FLOAT)\
THROW_ON_MISMATCH(val, CHAR)

void Cell::set_value(const std::any& value)
{
    THROW_IF_NOT_SQL_TYPE(value);
    value_ = value;
}

const void * Cell::to_raw() const
{
    return std::any_cast<void>(&value_);
}
void Cell::from_raw(const void *data)
{
    void *d = malloc(size_);
    memcpy(d, data, size_);
    switch (type_)
    {
        case DataType::INTEGER:
            value_ = *static_cast<cell_type_v<DataType::INTEGER>*>(d);
            break;
        case DataType::FLOAT:
            value_ = *static_cast<cell_type_v<DataType::FLOAT>*>(d);
            break;
        case DataType::CHAR:
            value_ = *static_cast<cell_type_v<DataType::CHAR>*>(d);
            break;
        default:
            throw std::runtime_error("from_raw() failed");
    }
    free(d);
}

uint32_t Cell::get_size() const
{
    return size_;
}
void Cell::set_size(uint32_t new_size)
{
    size_ = new_size;
}
