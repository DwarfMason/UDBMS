//
// Created by veto on 08.10.19.
//

#ifndef CREATESTATEMENT_H
#define CREATESTATEMENT_H


#include <string>
#include <vector>
#include "BaseStatement.h"

struct CreateStatement : BaseStatement
{
    enum class Type{
        INTEGER,
        VARCHAR
    };
    enum flag{
        UNIQUE,
        PRIMARY,
        NOT_NULL
    };
    enum class Constraints{
        PRIMARY_KEY,
        UNIQUE
    };

    struct Column
    {
        std::string name;
        Type type;
        int typeLen = -1;
        std::vector<int> flags;
    };
    struct Constraint_expr
    {
        std::vector<std::string> keys;
        Constraints type;
    };
    struct Constraint
    {
        Constraint_expr constraint;
        std::string name = "";
    };
    struct Statement
    {
        std::vector<Column> columns;
        std::vector<Constraint> constraints;
        std::string tableName;
    };
};


#endif //CREATESTATEMENT_H
