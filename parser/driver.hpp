#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include "scanner.hpp"
#include "parser.tab.hh"
#include "../storage_engine.h"

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


    std::vector<std::string> list_vec;
    std::vector<std::pair<std::string,std::string>> decl_vec;

    void create_table(std::string name);

    void drop_table();

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
