#include "constraints.h"
bool constraints::is_not_null() const
{
    return not_null_;
}
bool constraints::is_unique_key() const
{
    return unique_;
}
bool constraints::is_index() const
{
    return index_;
}
void constraints::set_not_null(bool value)
{
    not_null_ = value;
}
void constraints::set_unique(bool value)
{
    unique_ = value;
}
void constraints::set_index(bool value)
{
    index_ = value;
}
bool constraints::is_primary_key() const
{
    return is_not_null() and is_unique_key();
}

