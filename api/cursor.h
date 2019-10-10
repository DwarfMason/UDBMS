#pragma once

#include "table.h"

class cursor
{
public:
    explicit cursor(const table& t);
    row next();
    void update(const std::map<std::string, void*>& kv);
    void remove();
private:
    table tbl_;
    std::vector<row>::iterator pos_;
};
