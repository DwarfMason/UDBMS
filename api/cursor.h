#pragma once

#include "table.h"

class cursor
{
public:
    // Create cursor for the table
    explicit cursor(const table& t);
    // Get current value and go next.
    // Throws cursor_eof_error if reached end of table.
    row next();
    // Update current row. Receives std::map with column name as a key
    // and void* as a value. You need to create a pointer to transmit value.
    void update(const std::map<std::string, void*>& kv);
    // Remove a row on current position.
    void remove();
private:
    table tbl_;
    std::vector<row>::iterator pos_;
};
