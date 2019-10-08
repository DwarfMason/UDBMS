#include "storage_engine.h"

namespace fs = std::filesystem;

const auto DATA_PATH = std::filesystem::current_path().string() + "/";
const std::string METADATA_EXT = ".meta";

table storage_engine::load_table(const std::string &name)
{
    throw std::logic_error("This function has been moved to the API section.");
}
table storage_engine::create_table(const std::string &name)
{
    throw std::logic_error("This function has been moved to the API section.");
}
void storage_engine::delete_table(const std::string &name)
{
    throw std::logic_error("This function has been moved to the API section.");
}
void storage_engine::save_table(const table &tbl)
{
    throw std::logic_error("This function has been moved to the API section.");
}
std::string storage_engine::show_create_table(const table &tbl)
{
    throw std::logic_error("This function should have been moved to the LOGIC section.");
    std::stringstream sql;
    sql << "CREATE TABLE `" << tbl.get_name() << "` (" << std::endl;
    const auto& cols = tbl.get_columns();
    std::vector<std::string> unique_keys, primary_keys;
    for (unsigned long i = 0; i < cols.size(); ++i) {
        const column& col = cols[i];
        sql << "  `" << col.get_name() << "` ";
        std::string type;
        switch (col.get_type()) {
            case data_type::INTEGER:
                type = "int";
                break;
            case data_type::FLOAT:
                type = "float";
                break;
            case data_type::VARCHAR:
                type = "varchar";
                break;
        }
        sql << type;
        auto size = col.get_size();
        if (col.get_size() > 0) {
            sql << "(" << size << ")";
        }
        const constraints& cts = col.get_constraints();
        if (cts.is_primary_key()) {
            sql << " PRIMARY KEY";
        } else {
            if (cts.is_not_null()) {
                sql << " NOT NULL";
            }
            if (cts.is_unique_key()) {
                sql << " UNIQUE";
            }
        }
        if (cts.is_index()) {
            sql << " INDEX";
        }
        if (i < cols.size()-1)
            sql << "," << std::endl;
    }
    sql << std::endl << ");";
    return sql.str();
}