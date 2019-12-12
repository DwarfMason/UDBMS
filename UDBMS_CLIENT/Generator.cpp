//
// Created by dwarf on 12.12.2019.
//

#include <iostream>
#include "Generator.h"
#include <ctime>


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
    std::srand(std::time(0));
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
    this->InsertNewNode("existing_column_names");

    //Array of free tables names
    this->InsertNewNode("column_names");
    this->InsertIntoNode("column_names", {"trembolo", "tam_tam", "ramp", "hydra", "PC", "atom", "ctrl", "id"});

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
    this->InsertIntoNode("random_strings", {"qwerty", "people", "quiz", "baldhead", "useless", "pointless",
                                            "zhizha", "cos", "sin"});

    //Random numbers
    this->InsertNewNode("random_nums");
    this->InsertIntoNode("random_nums", {"123", "15", "42", "10101010101", "2101999", "5081999", "16", "228",
                                         "15", "617", "65463"});

    //Types
    this->InsertNewNode("types");
    this->InsertIntoNode("types", {"int", "char", "float"});
}

std::string Generator::GetLastRequest() {
    return this->_last_request;
}

bool Generator::InsertNewNode(const std::string &&name) {
    if (this->_nodes.find(name) != this->_nodes.end()) return false;
    this->_nodes.emplace(name, GeneratorNode(name));
    return true;
}

void Generator::InsertIntoNode(const std::string &&name, std::initializer_list<std::string> list) {
    this->_nodes.at(name).InsertValue(list);
}

std::string Generator::GenerateRequest() {
    switch (random()%7) {
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
    return ("show create table " + GetName(true) + ";");
}

std::string Generator::DropTableReq() {
    return ("drop table " + GenerateSetOfNames(true) + ";");
}

std::string Generator::CreateTableReq() {
    std::string request = "create table " + GetName(true) + "(";
    request.append("\n" + GetName(false) + " " + GetSmth("types"));
    while (true) {
        if (FlipCoin()) break;
        request.append(",\n" + GetName(false) + " " + GetSmth("types"));
    }
    request.append(");");
    return request;
}

bool Generator::FlipCoin() {
    return random() % 2;
}

std::string Generator::GetName(bool table_column) {
    std::set<std::string> buff = this->_nodes.at(table_column ? "existing_table_names" :
                                                 "existing_column_names").GetValues();
    if (!buff.empty()) {
        int r = random() % buff.size();
        auto a = buff.begin();
        std::advance(a, r);
        return *(a);
    } else
        return GetSmth(table_column ? "table_names" : "column_names");
}


std::string Generator::GenerateSetOfNames(bool table_column) {
    std::string buff;
    this->_name_counts++;
    buff.append(GetName(table_column));
    if (FlipCoin()) {
        buff.append(", ");
        buff.append(GenerateSetOfNames(table_column));
    }
    return buff;
}

std::string Generator::GetSmth(std::string smth) {
    std::set<std::string> buff = this->_nodes.at(smth).GetValues();
    int r = random() % buff.size();
    auto a = buff.begin();
    std::advance(a, r);
    return *(a);
}

std::string Generator::SelectReq() {
    std::string request = "select ";
    if (FlipCoin()) request.append("*");
    else
        request.append(GenerateSetOfNames(false));
    request.append(" from ");
    request.append(GetName(true) + " ");
    if (FlipCoin()) request.append(";");
    else
        request.append("where" + CreateExpr() + ";");

    return request;
}

std::string Generator::CreateExpr() {
    std::string expression = " (";
    if (FlipCoin()) expression.append(NumericExpr() + " " + GetSmth("signs") + " " + NumericExpr() + ")");
    else
        expression.append(StrExpr() + " " + GetSmth("signs") + " " + StrExpr() + ")");
    if (FlipCoin()) expression.append(" " + GetSmth("logical_expr") + CreateExpr());
    return expression;
}

std::string Generator::NumericExpr() {
    std::string expr;
    if (FlipCoin()) expr = GetSmth("random_nums");
    else
        expr = GetName(false);

    if (FlipCoin()) {
        expr.append(" " + GetSmth("math_expr") + " " + NumericExpr());
        return expr;
    }
    return expr;
}

std::string Generator::StrExpr() {
    std::string expr;
    if (FlipCoin()) expr = GetSmth("random_strings");
    else
        expr = GetName(false);

    if (FlipCoin()) {
        expr.append(" " + GetSmth("math_expr") + " " + StrExpr());
        return expr;
    }
    return expr;
}

std::string Generator::InsertReq() {
    std::string request = "insert into ";
    this->_name_counts = 0;
    request.append(GetName(true) + " (" + GenerateSetOfNames(false) + ")");
    request.append(" values (" + GetSmth(FlipCoin() ? "random_strings" : "random_nums"));
    for (int i = 1; i < this->_name_counts; i++)
        request.append(", " + GetSmth(FlipCoin() ? "random_strings" : "random_nums"));
    request.append(");");
    return request;
}

std::string Generator::UpdateReq() {
    std::string request = "update " + GetName(true) + " set ";
    request.append(GenerateSetOfNames(false) + " = " +
                   GetSmth(FlipCoin() ? "random_strings" : "random_nums"));
    if (FlipCoin()) request.append(" where" + CreateExpr());
    request.append(";");
    return request;
}

std::string Generator::DeleteReq() {
    std::string request = "delete from " + GetName(false);
    if (FlipCoin()) request.append(" where" + CreateExpr());
    request.append(";");
    return request;
}

