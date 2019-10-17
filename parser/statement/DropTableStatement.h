//
// Created by veto on 08.10.19.
//

#ifndef DROPTABLESTATEMENT_H
#define DROPTABLESTATEMENT_H
#include <string>
#include <vector>
#include "BaseStatement.h"

struct DropTableStatement : public BaseStatement
{
    struct Statement {
        std::vector<std::string> keys;
    };
};
#endif //DROPTABLESTATEMENT_H
