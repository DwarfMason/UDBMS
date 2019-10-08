#include <fstream>

#include "driver.hpp"
#include "statement/BaseStatement.h"

UDBMS::Driver::~Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void UDBMS::Driver::parse(std::istream &stream )
{
   parse_helper( stream );
}


void UDBMS::Driver::parse_helper(std::istream &stream )
{

      delete(scanner);
      scanner = new UDBMS::Scanner(&stream );
      parser = new UDBMS::DParse((*scanner) /* scanner */,
                                    (*this) /* driver */ );
      parser->parse();
}
void UDBMS::Driver::create_table(std::string name)
{

}
void UDBMS::Driver::drop_table(std::vector<std::string> name_list_expr)
{

}
void UDBMS::Driver::show_create(std::string name)
{

}
void UDBMS::Driver::select(std::vector<std::string> select_expr_list,
                                  std::vector<std::string> name_list_expr,
                                  std::vector<bool> opt_where,
                                  std::vector<bool> opt_groupby,
                                  std::vector<bool> opt_having,
                                  std::vector<bool> opt_orderby,
                                  std::vector<bool> opt_limit,
                                  std::vector<bool> opt_into_list)
{

}
