#pragma once

#include <nlohmann/json.hpp>
#include <api/Column.h>

// TODO Documentation!!!

class TableMetadata
{
public:
    TableMetadata() = default;
    explicit TableMetadata(std::string name, std::vector<Column> columns);
    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string& new_name);
    [[nodiscard]] std::vector<Column> get_columns() const;
    [[nodiscard]] size_t get_index_by_name(const std::string& col_name) const;
    [[nodiscard]] std::string get_create_query() const;
private:
    std::string name_;
    // TODO add storage engine value one day
    std::vector<Column> cols_;
    std::map<std::string, size_t> col_index_;
};

namespace nlohmann
{
template <>
struct adl_serializer<TableMetadata>
{
    static TableMetadata from_json(const json& j)
    {
        auto name = j.at("name").get<std::string>();
        auto cols = j.at("columns").get<std::vector<Column>>();
        return TableMetadata(name, cols);
    }

    static void to_json(json& j, TableMetadata t)
    {
        j = {
            {"name", t.get_name()},
            {"columns", t.get_columns()}
        };
    }
};
}