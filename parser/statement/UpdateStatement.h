//
// Created by veto on 09.10.19.
//

#ifndef UPDATESTATEMENT_H
#define UPDATESTATEMENT_H
#include <string>
#include "BaseStatement.h"
struct UpdateStatement : BaseStatement
{
    struct Statement
    {
        std::string tableToUpdate;
        std::string columnName;
        std::string newValue;
        /* example
         * `id` = `qwe`
         * */
        std::pair<std::string,std::string> expr;
    };
};
#endif //UPDATESTATEMENT_H
