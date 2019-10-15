#include "Constraints.h"
bool Constraints::is_not_null() const
{
    return not_null_;
}
bool Constraints::is_unique_key() const
{
    return unique_;
}
bool Constraints::is_index() const
{
    return index_;
}
void Constraints::set_not_null(bool value)
{
    not_null_ = value;
}
void Constraints::set_unique(bool value)
{
    unique_ = value;
}
void Constraints::set_index(bool value)
{
    index_ = value;
}
bool Constraints::is_primary_key() const
{
    return is_not_null() and is_unique_key();
}

