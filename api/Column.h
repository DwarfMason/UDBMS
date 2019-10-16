#pragma once

#include <string>
#include <jsoncons/json.hpp>
#include "Constraints.h"
#include "data_type.h"

class Column
{
public:
    Column() = default;
    explicit Column(std::string name, data_type type);
    ~Column();
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] const data_type& get_type() const;
    [[nodiscard]] uint32_t get_size() const;
    [[nodiscard]] const Constraints& get_constraints() const;
    void set_name(std::string name);
    void set_type(data_type type);
    void set_size(uint32_t size);
    void set_constraints(Constraints cts);
private:
    std::string name_;
    data_type type_;
    uint32_t size_ = 0;
    Constraints constraints_;
};

JSONCONS_GETTER_SETTER_NAMED_TRAITS_DECL(Column,
                                         (get_name, set_name, "name"),
                                         (get_type, set_type, "type"),
                                         (get_size, set_size, "size"),
                                         (get_constraints, set_constraints, "Constraints"))