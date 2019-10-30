#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include <parser/statement/ShowCreateStatement.h>
#include <parser/statement/DropTableStatement.h>

#include "scanner.hpp"
#include "parser.tab.hh"
#include "api/v0/Cursor.h"
#include <api/v0/crud.h>
#include <parser/statement/InsertStatement.h>
#include <parser/statement/DeleteStatement.h>
#include <parser/statement/SelectStatement.h>
#include <parser/statement/UpdateStatement.h>
#include "statement/CreateStatement.h"

namespace UDBMS{

class Driver{
public:
   Driver() = default;

   virtual ~Driver();

   /**
    * parse - parse from a c++ input stream
    * @param is - std::istream&, valid input stream
    */
   void parse( std::stringstream &iss );

   /*
    * SELECT select_expr_list
    * FROM name_list_expr
    * opt_where
    * opt_groupby
    * opt_having
    * opt_orderby
    * opt_limit
    * opt_into_list;
    */

    void create_table(CreateStatement::Statement stmt);

    /*
    * DROP TABLE name_list_expr;
    */
    void drop_table(DropTableStatement::Statement stmt);

    /*
    * SHOW CREATE TABLE table_name;
    */
    void show_create(ShowCreateStatement::Statement stmt);

    void insert(InsertStatement::Statement stmt);
    void delete_stmt(DeleteStatement::Statement stmt);
    void select(SelectStatement::Statement stmt);
    void update(UpdateStatement::Statement stmt);
    bool unique = 0;
    bool constraint = false;

private:

   void parse_helper( std::stringstream &stream );

   UDBMS::DParse  *parser  = nullptr;
   UDBMS::Scanner *scanner = nullptr;

    class ValueManager{
        private:
            static void* createFoat(std::string val);
            static void* createChar(std::string val);
            static void* createInt(std::string val);
        public:
            static void* createPointer(std::string str, DataType type);

            static auto getFloat(void *v){return *static_cast<float*>(v);}
            static char* getChar(void *v,size_t size = 1){
                char * res = new char[size+1];
                for (int i = 0; i < size; ++i) {
                    res[i] = *(static_cast<char*>(v)+i);
                }
                res[size] = '\0';
                return res;
            ;}
            static int getInt(void *v){return *static_cast<int*>(v);}
    };

};

} /* end namespace UDBMS */
#endif /* END __DRIVER_HPP__ */
