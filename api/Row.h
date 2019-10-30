#pragma once

#include <vector>
#include <cstdint>
#include <api/Cell.h>

/**
 * @brief A row class that stores Cell objects.
 *
 * Has getter and setter for specific cell.
 */
class Row
{
public:
    explicit Row(const std::vector<Cell>& cells);
    [[nodiscard]] bool is_deleted() const;
    void mark_as_deleted();
    void unmark_as_deleted();
    void set_at(size_t index, const std::any& value);
    [[nodiscard]] Cell get_at(size_t index) const;
    [[nodiscard]] uint32_t get_size() const;
private:
    std::vector<Cell> cells_;
    bool deleted_;
    // Further flags here...
};
