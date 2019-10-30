#pragma once

#include <cstdint>
#include <any>
#include <api/DataType.h>

/**
 * @brief A cell class.
 *
 * Uses std::any to contain a value.
 */
class Cell
{
public:
    /**
     * Create an empty cell.
     * @param type Cell type.
     * @param size Cell size.
     */
    explicit Cell(DataType type, uint32_t size);
    /**
     * Create a cell with defined value.
     * @param type Cell type.
     * @param size Cell size.
     * @param value Cell value.
     */
    explicit Cell(DataType type, uint32_t size, const std::any& value);
    /**
     * Set a cell value.
     * @param value New cell value.
     */
    void set_value(const std::any& value);
    /**
     * Get cell value with type `T`.
     * @tparam T Return type.
     * @return Cell value with defined type.
     */
    template <typename T>
    [[nodiscard]] T get_value() const
    {
        // TODO handle bad_any_cast exception
        return std::any_cast<T>(value_);
    }
    /**
     * Get cell value as raw data.
     * @return A pointer to raw cell data.
     */
    [[nodiscard]] const void * to_raw() const;
    /**
     * Set cell value from raw data.
     * @param data A raw data.
     */
    void from_raw(const void* data);
    /**
     * Get cell physical size
     * @return Cell physical size.
     */
    [[nodiscard]] uint32_t get_size() const;
    /**
     * Set cell physical size.
     * @param new_size New physical size of cell.
     */
    void set_size(uint32_t new_size);
private:
    DataType type_; /**< A type of cell. */
    uint32_t size_; /**< A size of cell. */
    std::any value_; /**< A cell value. */
};


