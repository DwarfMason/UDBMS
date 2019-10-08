#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include "scanner.hpp"
#include "parser.tab.hh"
#include "../storage_engine.h"
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
   void parse( std::istream &iss );

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
    void select(
        std::vector<std::string> select_expr_list,
        std::vector<std::string> name_list_expr,
        std::vector<bool> opt_where,
        std::vector<bool> opt_groupby,
        std::vector<bool> opt_having,
        std::vector<bool> opt_orderby,
        std::vector<bool> opt_limit,
        std::vector<bool> opt_into_list
        );


    void create_table(std::string name);

    /*
    * DROP TABLE name_list_expr;
    */
    void drop_table(std::vector<std::string> name_list_expr);

    /*
    * SHOW CREATE TABLE table_name;
    */
    void show_create(std::string name);
    bool unique = 0;
    bool constraint = false;

private:

   void parse_helper( std::istream &stream );

   UDBMS::DParse  *parser  = nullptr;
   UDBMS::Scanner *scanner = nullptr;

};

} /* end namespace UDBMS */
#endif /* END __DRIVER_HPP__ */
