#pragma once

#include "table.h"

class cursor
{
public:
    explicit cursor(table& t);
    //next
    //update
    //delete
private:
    //table& tbl_;
    std::vector<row>::iterator pos_;
};
