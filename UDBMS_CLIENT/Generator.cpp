//
// Created by dwarf on 12.12.2019.
//

#include "Generator.h"


bool GeneratorNode::InsertValue(std::initializer_list<std::string> values) {
    this->_values.insert(values);
    return true;
}

bool GeneratorNode::DeleteValue(const std::string &&value) {
    return !this->_values.erase(value);
}

std::string GeneratorNode::GetName() {
    return this->_name;
}

bool GeneratorNode::GetIfRepeatable() {
    return this->_repeatable;
}

std::set<std::string> GeneratorNode::GetValues() {
    return this->_values;
}

Generator::Generator() {
    //Vector of commands
    this->_commands.insert(this->_commands.end(), {"drop table", "select", "create table", "insert into",
                                                   "show create table", "update", "delete from"});

    //Array of existing tables names
    this->InsertNewNode("existing_table_names");

    //Array of free tables names
    this->InsertNewNode("table_names");
    this->InsertIntoNode("table_names", {"a", "b", "c", "d", "table_create", "stranger", "something",
                                         "zachet", "F", "pity"});

    //Array of existing columns names
    this->InsertNewNode("existing_columns_names");

    //Array of free tables names
    this->InsertNewNode("column_names");
    this->InsertIntoNode("column_names", {"a", "b", "c", "d", "table_create", "stranger", "something",
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

bool Generator::InsertNewNode(const std::string &&name) {
    if (this->_nodes.find(name) == this->_nodes.end()) return false;
    this->_nodes.emplace(name, GeneratorNode(name));
    return true;
}

void Generator::InsertIntoNode(const std::string &&name, std::initializer_list<std::string> list) {
    this->_nodes.find(name)->second.InsertValue(list);
}

std::string Generator::GenerateRequest() {
    switch (4) {
        case 0:
            return DropTableReq();
            break;
        case 1:
            return SelectReq();
            break;
        case 2:
            return CreateTableReq();
            break;
        case 3:
            return InsertReq();
            break;
        case 4:
            return ShowCreateReq();
            break;
        case 5:
            return UpdateReq();
            break;
        case 6:
            return DeleteReq();
            break;
    }

    return "¯\\_(ツ)_/¯";
}

std::string Generator::ShowCreateReq() {
    return ("show create table" + GetName(true) + "{");
}

std::string Generator::DropTableReq() {
    return ("drop table" + GenerateSetOfNames(true));
}

std::string Generator::CreateTableReq() {
    std::string request = "create table " + GetName(true) + "{";
    request.append("\n" + GetName(false) + " " + GetSmth("types"));
    while(true) {
        if (FlipCoin()) break;
        request.append(",\n" + GetName(false) + " " + GetSmth("types"));
    }
    request.append("};");
    return request;
}

bool Generator::FlipCoin() {
    return rand() % 2;
}

std::string Generator::GetName(bool table_column) {
    std::set<std::string> buff = this->_nodes.find(table_column ? "existing_table_names" :
                                                   "existing_column_names")->second.GetValues();
    if (!buff.empty()) {
        int r = rand() % buff.size();
        auto a = buff.begin();
        std::advance(a, r);
        return *(a);
    } else
        return GetSmth(table_column ? "table_names" : "column_names");
}


std::string Generator::GenerateSetOfNames(bool table_column) {
    std::string buff;
    buff.append(GetName(table_column));
    if (FlipCoin()) {
        buff.append(", ");
        buff.append(GenerateSetOfNames(table_column));
    }
    return buff;
}

std::string Generator::GetSmth(std::string smth) {
    std::set<std::string> buff = this->_nodes.find(smth)->second.GetValues();
    int r = rand() % buff.size();
    auto a = buff.begin();
    std::advance(a, r);
    return *(a);
}

