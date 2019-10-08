#include <cstdint>
#include <jsoncons/json.hpp>

enum class data_type: uint8_t
{
    INTEGER,
    FLOAT,
    VARCHAR,
};

JSONCONS_ENUM_TRAITS_DECL(data_type, INTEGER, FLOAT, VARCHAR);