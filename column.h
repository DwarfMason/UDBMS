#pragma once

#include <string>
#include <jsoncons/json.hpp>
#include "constraints.h"
#include "data_type.h"

class column
{
public:
    column() = default;
    explicit column(std::string name, data_type type);
    ~column();
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] const data_type& get_type() const;
    [[nodiscard]] uint32_t get_size() const;
    [[nodiscard]] const constraints& get_constraints() const;
    void set_name(std::string name);
    void set_type(const data_type& type);
    void set_size(uint32_t size);
    void set_constraints(const constraints& cts);
private:
    std::string name_;
    data_type type_;
    uint32_t size_;
    constraints constraints_;
};

JSONCONS_GETTER_SETTER_NAMED_TRAITS_DECL(column,
                                         (get_name, set_name, "name"),
                                         (get_type, set_type, "type"),
                                         (get_size, set_size, "size"),
                                         (get_constraints, set_constraints, "constraints"))