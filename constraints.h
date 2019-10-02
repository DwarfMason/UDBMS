#pragma once

#include <jsoncons/json.hpp>

class constraints
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

JSONCONS_GETTER_SETTER_NAMED_TRAITS_DECL(constraints,
                                         (is_not_null, set_not_null, "not_null"),
                                         (is_unique_key, set_unique, "unique"),
                                         (is_index, set_index, "index"))