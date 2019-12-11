//
// Created by dwarf on 12.12.2019.
//

#ifndef UDBMS_GENERATOR_H
#define UDBMS_GENERATOR_H


#include <string>
#include <utility>
#include <set>
#include <map>

class GeneratorNode{
public:
    explicit GeneratorNode(std::string name): _name(std::move(name)){};
    ~GeneratorNode() = default;

    std::string GetName();
    bool InsertValue(std::initializer_list<std::string>);
    bool DeleteValue(const std::string&);
    bool GetIfRepeatable();

private:
    std::string _name;
    std::set<std::string> _values;
    bool _repeatable = false;
};

class Generator {
public:
    Generator();
    ~Generator() = default;

    bool InsertNewNode(const std::string&);
    void InsertIntoNode(const std::string&, std::initializer_list<std::string>);
    std::string GetLastRequest();

private:
    std::map<std::string, GeneratorNode> _nodes;
    std::string _last_request = " ";
};
#endif //UDBMS_GENERATOR_H
