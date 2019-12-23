//
// Created by veto on 09.10.19.
//

#ifndef UPDATESTATEMENT_H
#define UPDATESTATEMENT_H
#include <string>
#include <any>
#include "BaseStatement.h"
struct UpdateStatement : public BaseStatement
{
    struct Statement
    {
        std::string tableToUpdate;
        std::string columnName;
        std::any newValue;
        /* example
         * `id` = `qwe`
         * */
        std::pair<std::string,std::any> expr;
    };
};
#endif //UPDATESTATEMENT_H
