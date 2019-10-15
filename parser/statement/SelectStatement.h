//
// Created by veto on 09.10.19.
//

#ifndef SELECTSTATEMENT_H
#define SELECTSTATEMENT_H
#include <string>
#include <vector>
#include "BaseStatement.h"
struct SelectStatement : BaseStatement
{
    struct Statement
    {
        std::string name;
        std::vector<std::string> selector;
        /* example
         * `id` = `qwe`
         * */
        std::pair<std::string,std::string> expr;
    };
};
#endif //SELECTSTATEMENT_H
