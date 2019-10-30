#include "TableFileWorker.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <constants.h>
#include <exceptions.h>
#include <api/data_wrap/YDBDataWrapper.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

TableMetadata TableFileWorker::create_table(const std::string &name, const std::vector<Column> &cols)
{
    auto meta_filename = name + METADATA_EXT;
    auto storage_fname = name + STORAGE_EXT;
    fs::path metadata_path(DATA_PATH / meta_filename);
    fs::path storage_path(DATA_PATH / storage_fname);

    if (!fs::exists(metadata_path)) {
        TableMetadata t(name, cols);
        YDBDataWrapper w(t, storage_path.string());
        json meta_json(t);
        std::ofstream file(metadata_path);
        file << std::setw(4) << meta_json << std::endl;
        file.close();
        return t;
    }
    else
        throw table_exist_error();
}

TableMetadata TableFileWorker::load_table(const std::string &name)
{
    auto meta_filename = name + METADATA_EXT;
    fs::path metadata_path(DATA_PATH / meta_filename);

    if (fs::exists(metadata_path)) {
        json meta_json;
        std::ifstream file(metadata_path);
        file >> meta_json;
        return meta_json.get<TableMetadata>();
    }
    else
        throw table_not_exist_error();
}

void TableFileWorker::drop_table(const TableMetadata &tbl)
{
    auto meta_filename = tbl.get_name() + METADATA_EXT;
    auto storage_fname = tbl.get_name() + STORAGE_EXT;
    fs::path metadata_path(DATA_PATH / meta_filename);
    fs::path storage_path(DATA_PATH / storage_fname);

    if (!fs::remove(metadata_path))
        throw table_not_exist_error();
    fs::remove(storage_path);
}
BaseDataWrapper* TableFileWorker::load_table_data(const TableMetadata &tbl)
{
    auto storage_fname = tbl.get_name() + STORAGE_EXT;
    fs::path storage_path(DATA_PATH / storage_fname);

    return new YDBDataWrapper(tbl, storage_path.string());
}
