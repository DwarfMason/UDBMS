//
// Created by dwarf on 12.12.2019.
//

#include "Generator.h"


bool GeneratorNode::InsertValue(std::initializer_list<std::string> values) {
    this->_values.insert(values);
    return true;
}

bool GeneratorNode::DeleteValue(const std::string& value) {
    return !this->_values.erase(value);
}

std::string GeneratorNode::GetName() {
    return this->_name;
}

bool GeneratorNode::GetIfRepeatable() {
    return this->_repeatable;
}

Generator::Generator() {
    //Entry command words
    this->InsertNewNode("first_word");
    this->InsertIntoNode("first_word", {"drop table", "select", "create table", "insert into",
                                        "show create table", "update", "delete from", });

    //Array of existing tables names
    this->InsertNewNode("existing_table_names");

    //Array of free tables names
    this->InsertNewNode("table_names");
    this->InsertIntoNode("table_names", {"a", "b", "c", "d", "table", "stranger", "something",
                                         "zachet", "F", "pity"});

    //Array of existing columns names
    this->InsertNewNode("existing_columns_names");

    //Array of free tables names
    this->InsertNewNode("column_names");
    this->InsertIntoNode("column_names", {"a", "b", "c", "d", "table", "stranger", "something",
                                         "zachet", "F", "pity"});

    //Left set bracket
    this->InsertNewNode("left_set_bracket");
    this->InsertIntoNode("left_set_bracket", {"{"});

    //Right set bracket
    this->InsertNewNode("right_set_bracket");
    this->InsertIntoNode("right_set_bracket", {"}"});

    //Left bracket
    this->InsertNewNode("left_bracket");
    this->InsertIntoNode("left_bracket", {"("});

    //Right bracket
    this->InsertNewNode("right_bracket");
    this->InsertIntoNode("right_bracket", {")"});

    //Semi-colon
    this->InsertNewNode("semi_colon");
    this->InsertIntoNode("semi_colon", {";"});

    //Coma
    this->InsertNewNode("coma");
    this->InsertIntoNode("coma", {","});

    //Wildcard
    this->InsertNewNode("wildcard");
    this->InsertIntoNode("wildcard", {"*"});

    //From
    this->InsertNewNode("from");
    this->InsertIntoNode("from", {"from"});

    //Signs
    this->InsertNewNode("signs");
    this->InsertIntoNode("signs", {">", "<", "=", "!=", ">=", "<="});
    
    //Math expr
    this->InsertNewNode("math_expr");
    this->InsertIntoNode("math_expr", {"*", "/", "+", "-"});

    //Logical expr
    this->InsertNewNode("logical_expr");
    this->InsertIntoNode("logical_expr", {"AND", "OR"});

    //WHERE
    this->InsertNewNode("where");
    this->InsertIntoNode("where", {"WHERE"});

    //Set
    this->InsertNewNode("set");
    this->InsertIntoNode("set", {"set"});

    //Random strings
    this->InsertNewNode("random_strings");
    this->InsertIntoNode("random_strings", {"qwerty", "people", "quiz", "boldhead", "useless", "pointless"});

    //Random numbers
    this->InsertNewNode("random_nums");
    this->InsertIntoNode("random_nums", {"123", "15", "42", "10101010101", "02101999", "05081999"});

    //Types
    this->InsertNewNode("types");
    this->InsertIntoNode("types", {"int, char"});
}

std::string Generator::GetLastRequest() {
    return this->_last_request;
}

bool Generator::InsertNewNode(const std::string& name) {
    if (this->_nodes.find(name) == this->_nodes.end()) return false;
    this->_nodes.emplace(name, GeneratorNode(name));
    return true;
}

void Generator::InsertIntoNode(const std::string& name, std::initializer_list<std::string> list) {
    this->_nodes.find(name)->second.InsertValue(list);
}
