#pragma once

#include <string>
#include <api/Row.h>
#include <api/TableMetadata.h>

class BaseDataWrapper
{
public:
    explicit BaseDataWrapper(const TableMetadata& table, const std::string& filename);
    virtual ~BaseDataWrapper() = default;
    virtual void insert_row(const Row& row) = 0;
    [[nodiscard]] virtual Row get_current_row() const = 0;
    virtual void delete_current_row() = 0;
    virtual void update_current_row(const std::map<std::string, std::any>& kv) = 0;
    virtual void next_row() = 0;
protected:
    std::string filename_;
    TableMetadata metadata_;
};
