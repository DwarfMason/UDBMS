#pragma once

#include "Table.h"

class Cursor
{
public:
    // Create Cursor for the Table
    explicit Cursor(const Table& t);
    // Get current value and go next.
    // Throws cursor_eof_error if reached end of Table.
    Row next();
    // Update current Row. Receives std::map with Column name as a key
    // and void* as a value. You need to create a pointer to transmit value.
    void update(const std::map<std::string, void*>& kv);
    // Remove a Row on current position.
    void remove();
private:
    Table tbl_;
    std::vector<Row>::iterator pos_;
};