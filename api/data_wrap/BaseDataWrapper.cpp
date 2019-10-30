#include "BaseDataWrapper.h"
BaseDataWrapper::BaseDataWrapper(const TableMetadata& table, const std::string &filename)
{
    filename_ = filename;
    metadata_ = table;
}
