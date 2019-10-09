#include "data_type.h"

std::map<data_type, type_info> type_registry = {
    REGISTER_TYPE(INTEGER, 4),
    REGISTER_TYPE(FLOAT, 4),
    REGISTER_TYPE(CHAR, 1),
};