#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/cbor/cbor.hpp>
#include <jsoncons_ext/jsonpath/json_query.hpp>
#include "table.h"
#include "exceptions.h"

class storage_engine
{
public:
    static table load_table(const std::string &name);
    static table create_table(const std::string &name);
    static void delete_table(const std::string &name);
};
