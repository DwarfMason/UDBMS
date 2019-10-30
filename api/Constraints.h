#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Constraints
{
public:
    [[nodiscard]] bool is_not_null() const;
    [[nodiscard]] bool is_unique_key() const;
    [[nodiscard]] bool is_index() const;
    [[nodiscard]] bool is_primary_key() const;
    void set_not_null(bool value);
    void set_unique(bool value);
    void set_index(bool value);
private:
    bool not_null_ = false;
    bool unique_ = false;
    bool index_ = false;
};

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
