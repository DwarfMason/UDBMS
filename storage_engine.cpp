#include "storage_engine.h"

namespace fs = std::filesystem;

const auto METADATA_STORAGE = std::filesystem::current_path().string() + "/";
const std::string METADATA_EXT = ".meta";

table storage_engine::load_table(const std::string &name)
{
    fs::path metadata_path(METADATA_STORAGE + name + METADATA_EXT);
    if (fs::exists(metadata_path)) {
        jsoncons::json json;
        std::ifstream file(metadata_path);
        json = jsoncons::json::parse(file);
        return json.as<table>();
    }
    else
        throw table_not_exist_error();
}
table storage_engine::create_table(const std::string &name)
{
    fs::path metadata_path(METADATA_STORAGE + name + METADATA_EXT);
    if (!fs::exists(metadata_path)) {
        table t(name);
        jsoncons::json json(t);
        std::ofstream file(metadata_path);
        file << jsoncons::pretty_print(json);
        file.close();
        return t;
    }
    else
        throw table_exist_error();
}
void storage_engine::delete_table(const std::string &name)
{
    fs::path metadata_path(METADATA_STORAGE + name + METADATA_EXT);

    if (!fs::remove(metadata_path))
        throw table_not_exist_error();
}
void storage_engine::save_table(const table &tbl)
{
    fs::path metadata_path(METADATA_STORAGE + tbl.get_name() + METADATA_EXT);
    if (fs::exists(metadata_path)) {
        jsoncons::json json(tbl);
        std::ofstream file(metadata_path);
        file << jsoncons::pretty_print(json);
    }
    else
        throw table_not_exist_error();
}
std::string storage_engine::show_create_table(const table &tbl)
{
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