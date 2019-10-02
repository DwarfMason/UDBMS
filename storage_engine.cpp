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
