#include "crud.h"

#include <fstream>
#include <jsoncons/json.hpp>
#include <constants.h>
#include <exceptions.h>

namespace fs = std::filesystem;

table API::create_table(const std::string &name)
{
    auto meta_filename = name + METADATA_EXT;
    fs::path metadata_path(DATA_PATH / meta_filename);

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

table API::load_table(const std::string &name)
{
    auto meta_filename = name + METADATA_EXT;
    fs::path metadata_path(DATA_PATH / meta_filename);

    if (fs::exists(metadata_path)) {
        jsoncons::json json;
        std::ifstream file(metadata_path);
        json = jsoncons::json::parse(file);
        return json.as<table>();
    }
    else
        throw table_not_exist_error();
}

void API::drop_table(const table &tbl)
{
    auto meta_filename = tbl.get_name() + METADATA_EXT;
    auto data_filename = tbl.get_name() + STORAGE_EXT;
    fs::path metadata_path(DATA_PATH / meta_filename);
    fs::path storage_path(DATA_PATH / data_filename);

    if (!fs::remove(metadata_path))
        throw table_not_exist_error();
    fs::remove(data_filename);
}

void API::commit_table(table &tbl)
{
    // TODO #1: save data on commit
    // TODO #2: think about marking some chunks as "dirty" and write only them

    auto meta_filename = tbl.get_name() + METADATA_EXT;
    fs::path metadata_path(DATA_PATH / meta_filename);

    if (fs::exists(metadata_path)) {
        jsoncons::json json(tbl);
        std::ofstream file(metadata_path);
        file << jsoncons::pretty_print(json);
        tbl.get_data().purge();
        auto a = tbl.get_rows();
        uint64_t row_size = 0;
        size_t offset = sizeof(uint64_t);
        for (const auto& c : tbl.get_columns()) {
            row_size += c.get_size();
        }
        for (row& r : a)
        {
            tbl.get_data().push_row(row_size);
            tbl.get_data().write_some(offset += row_size, row_size, r.get_data());
        }
    }
    else
        throw table_not_exist_error();
}
