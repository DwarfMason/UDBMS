//
// Created by veto on 08.10.19.
//

#ifndef CREATESTATEMENT_H
#define CREATESTATEMENT_H


#include <string>
#include <vector>
#include <iostream>
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

    static void get_str(Statement stmt){
        std::clog << "table name: " << stmt.tableName<< '\n';
        std::clog << "columns: " << '\n';
        for (int i = 0; i < stmt.columns.size(); ++i) {
            std::clog << "\tcolumn name: " << stmt.columns[i].name << "\n";
            std::clog << "\t\tflags: " << stmt.columns[i].flags.size() << " : ";
            for (int j = 0; j < stmt.columns[i].flags.size(); ++j){
                std::clog << std::to_string(stmt.columns[i].flags[j]);
            }
            std::clog <<'\n';
        }
        std::clog << "\tconstraints: " << '\n';
        for (int j = 0; j < stmt.constraints.size(); ++j) {
            std::clog << "\t\tconstraint name: " << stmt.constraints[j].name << '\n';
            std::clog << "\t\tconstraint flags: ";
            for (int k = 0; k < stmt.constraints[j].constraint.keys.size(); ++k) {
                std::clog << " "<< stmt.constraints[j].constraint.keys[k] << " ";
            }
            std::clog <<'\n';
        }
    }
};


#endif //CREATESTATEMENT_H
