#ifndef CONDITION_H
#define CONDITION_H
#include <string>
#include <functional>
#include <map>
#include <api/DataType.h>
#include <api/TableMetadata.h>
#include <api/Row.h>
#include "exceptions.h"

struct Node {
    bool isOperator = false;
    bool isVariable = false;
    Node *left = nullptr;
    Node *right = nullptr;
    std::string value;
    DataType type;
};


bool eq(Node *l, Node *r){
    if (l->type == DataType::FLOAT && r->type == DataType::FLOAT){

    }
    else if (l->type == DataType::INTEGER && r->type == DataType::FLOAT){

    }
    else if (l->type == DataType::FLOAT && r->type == DataType::INTEGER){

    }
    else if (l->type == DataType::INTEGER && r->type == DataType::INTEGER){

    }
    else if (l->type == DataType::CHAR && r->type == DataType::CHAR){

    } else throw custom_exception(305,"invalid type expresion");
}

Node *calculate(Node *n,const TableMetadata& table, Row& row){
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
                if (n->type == DataType::INTEGER) {
                    n->value = std::to_string(row.get_at(i).get_value<cell_type_v<DataType::INTEGER>>());
                    n->isVariable = false;
                }
                if (n->type == DataType::CHAR) {
                    n->value = row.get_at(i).get_value<cell_type_v<DataType::CHAR>>();
                    n->isVariable = false;
                }
                if (n->type == DataType::FLOAT) {
                    n->value = std::to_string(row.get_at(i).get_value<cell_type_v<DataType::FLOAT>>());
                    n->isVariable = false;
                }
                return n;
            }
        }
        throw custom_exception(304,"no such column");
    }

}
#endif //CONDITION_H
