//
// Created by veto on 08.10.19.
//

#ifndef TYPE_INCLUDE_H
#define TYPE_INCLUDE_H

#include <string>
#include <f2fs_fs.h>


class Create_statement{
    struct Param_flags
    {
        bool NOT_NULL = false;
        bool UNIQUE = false;
        bool PRIMARY_KEY = false;
        bool flags[3] {false, false, false};

        void merge(Param_flags a, Param_flags b){
            for (int i = 0; i < 3; ++i) {
                flags[i] = max(a.flags[i], b.flags[i]);
            }
        }
    };

    struct Create_single_param
    {
        std::string name;
        std::string var_type;
        int var_len = 0;
        Param_flags param_flags;
    };

    struct Constraints_expr
    {
        std::string type;
        std::string
    };

    struct Additional_params
    {
        std::string constraint_name;
        Constraints_expr constraints_expr;
    };

    struct Create_params
    {
        Create_single_param create_single_param;
        Additional_params additional_params;
    };
};

#endif //TYPE_INCLUDE_H
