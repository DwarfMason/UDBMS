#ifndef CONDITION_H
#define CONDITION_H
#include <string>
#include <functional>
#include <map>

#include "api/Table.h"
#include "exceptions.h"

struct Node {
    bool isOperator = false;
    bool isVariable = false;
    Node *left = nullptr;
    Node *right = nullptr;
    std::string value;
    data_type type;
};


bool eq(Node *l, Node *r){
    if (l->type == data_type::FLOAT && r->type == data_type::FLOAT){

    }
    else if (l->type == data_type::INTEGER && r->type == data_type::FLOAT){

    }
    else if (l->type == data_type::FLOAT && r->type == data_type::INTEGER){

    }
    else if (l->type == data_type::INTEGER && r->type == data_type::INTEGER){

    }
    else if (l->type == data_type::CHAR && r->type == data_type::CHAR){

    } else throw custom_exception(305,"invalid type expresion");
}

Node *calculate(Node *n,const Table& table,Row& row){
    if(n->isOperator){
        if (n->value == "<"){
            if (!n->left->isOperator){
                n->left = calculate(n->left,table,row);
            }
            if (!n->right->isOperator){
                n->right = calculate(n->right,table,row);
            }
//            n->value = std::to_string();
            n->isOperator = false;
        }
    }
    if(n->isVariable){
        std::vector<Column> columns = table.get_columns();
        for (int i = 0; i < columns.size(); ++i) {
            if (n->value == columns[i].get_name()) {
                n->type = columns[i].get_type();
                if (n->type == data_type::INTEGER) {
                    n->value = std::to_string(*static_cast<int *>(row.at(i)));
                    n->isVariable = false;
                }
                if (n->type == data_type::CHAR) {
                    n->value = std::to_string(*static_cast<char *>(row.at(i)));
                    n->isVariable = false;
                }
                if (n->type == data_type::FLOAT) {
                    n->value = std::to_string(*static_cast<float *>(row.at(i)));
                    n->isVariable = false;
                }
                return n;
            }
        }
        throw custom_exception(304,"no such column");
    }

}
#endif //CONDITION_H
