#ifndef CONDITION_H
#define CONDITION_H
#include <string>

namespace cond{
    enum ConditionType{
        ADD,
        SUB,
        MUL,
        DIV,
    /*compare*/
        EQUALS,
        NOT_EQUALS,
        LESS_EQUALS,
        GREATER_EQUALS,
        GREATER,
        LESS,
    /*logic*/
        AND,
        OR
    };
}

class Condition
{
    static void add(std::string a,std::string b);
};


#endif //CONDITION_H
