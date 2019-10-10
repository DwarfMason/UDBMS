#include <exceptions.h>
#include "cursor.h"

cursor::cursor(const table& t)
{
    tbl_ = t;
    pos_ = tbl_.get_rows().begin();
}

row cursor::next()
{
    auto ret = pos_;
    if (pos_ != tbl_.get_rows().end())
        ++pos_;
    else
        throw cursor_eof_error();
    return *ret;
}
void cursor::update(const std::map<std::string, void *> &kv)
{
    tbl_.set_cell_values(*pos_, kv);
}

void cursor::remove()
{
    tbl_.delete_row(pos_);
}
