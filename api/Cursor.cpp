#include <exceptions.h>
#include "Cursor.h"

Cursor::Cursor(const Table& t)
{
    tbl_ = t;
    pos_ = tbl_.get_rows().begin();
}

Row Cursor::next()
{
    auto ret = pos_;
    if (pos_ != tbl_.get_rows().end())
        ++pos_;
    else
        throw cursor_eof_error();
    return *ret;
}
void Cursor::update(const std::map<std::string, void *> &kv)
{
    tbl_.set_cell_values(*pos_, kv);
}

void Cursor::remove()
{
    if (pos_ != tbl_.get_rows().end())
        tbl_.delete_row(pos_);
    else
        throw cursor_eof_error();
}
