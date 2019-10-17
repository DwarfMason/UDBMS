//
// Created by veto on 09.10.19.
//

#ifndef DELETESTATEMENT_H
#define DELETESTATEMENT_H
#include <string>
#include "BaseStatement.h"
struct DeleteStatement : public BaseStatement
{
    struct Statement
    {
        std::string name;
        /* example
         * `id` = `qwe`
         * */
        std::pair<std::string,std::string> expr;
    };
};
#endif //DELETESTATEMENT_H
