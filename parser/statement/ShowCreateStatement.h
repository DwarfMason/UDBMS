//
// Created by veto on 08.10.19.
//

#ifndef SHOWCREATESTATEMENT_H
#define SHOWCREATESTATEMENT_H

#include <string>
#include "BaseStatement.h"

struct ShowCreateStatement : public BaseStatement
{
    struct Statement
    {
        std::string name;
    };
};

#endif //SHOWCREATESTATEMENT_H
