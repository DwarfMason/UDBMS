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
void UDBMS::Driver::create_table(CreateStatement::Statement stmt)
{

}
void UDBMS::Driver::drop_table(DropTableStatement::Statement stmt)
{

}
void UDBMS::Driver::show_create(ShowCreateStatement::Statement stmt)
{

}

