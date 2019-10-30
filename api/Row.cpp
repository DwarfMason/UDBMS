#include "Row.h"

Row::Row(const std::vector<Cell> &cells)
{
    cells_ = cells;
}
bool Row::is_deleted() const
{
    return deleted_;
}
void Row::mark_as_deleted()
{
    deleted_ = true;
}
void Row::unmark_as_deleted()
{
    deleted_ = false;
}

void Row::set_at(size_t index, const std::any& value)
{
    cells_.at(index).set_value(value);
}
Cell Row::get_at(size_t index) const
{
    return cells_.at(index);
}
uint32_t Row::get_size() const
{
    uint32_t result = 0;
    for (const auto& c : cells_)
    {
        result += c.get_size();
    }
    return result;
}
