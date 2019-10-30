#include "Constraints.h"

using json = nlohmann::json;

bool Constraints::is_not_null() const
{
    return not_null_;
}
bool Constraints::is_unique_key() const
{
    return unique_;
}
bool Constraints::is_index() const
{
    return index_;
}
void Constraints::set_not_null(bool value)
{
    not_null_ = value;
}
void Constraints::set_unique(bool value)
{
    unique_ = value;
}
void Constraints::set_index(bool value)
{
    index_ = value;
}
bool Constraints::is_primary_key() const
{
    return is_not_null() and is_unique_key();
}

void to_json(json& j, const Constraints& c) {
    j = json{
        {"not_null", c.is_not_null()},
        {"unique", c.is_unique_key()},
        {"index", c.is_index()}
    };
}

void from_json(const json& j, Constraints& c) {
    c.set_not_null(j.at("not_null").get<bool>());
    c.set_unique(j.at("unique").get<bool>());
    c.set_index(j.at("index").get<bool>());
}
