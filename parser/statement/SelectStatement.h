//
// Created by veto on 09.10.19.
//

#ifndef SELECTSTATEMENT_H
#define SELECTSTATEMENT_H
#include <string>
#include <vector>
#include <any>
#include "BaseStatement.h"
struct SelectStatement : public BaseStatement
{
    struct Statement
    {
        std::string name;
        std::vector<std::string> selector;
        /* example
         * `id` = `qwe`
         * */
        std::pair<std::string,std::any> expr;
        bool useExpr = false;
    };
};
#endif //SELECTSTATEMENT_H
