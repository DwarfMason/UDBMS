//
// Created by veto on 09.10.19.
//

#ifndef INSERTSTATEMENT_H
#define INSERTSTATEMENT_H
#include <string>
#include <vector>
#include "BaseStatement.h"

struct InsertStatement : public BaseStatement
{
    struct Statement
    {
        std::string name;
        std::vector<std::string> cols;
        std::vector<std::any> value;
    };
};
#endif //INSERTSTATEMENT_H
