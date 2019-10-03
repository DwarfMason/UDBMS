#include <cctype>
#include <fstream>
#include <cassert>

#include "driver.hpp"

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
    table t = storage_engine::create_table(name);
    std::vector<column> cols;
    for (int i = 0; i < decl_vec.size(); ++i) {
        auto col_type_str = decl_vec[i].first;
        data_type col_type;
        if (col_type_str == "int") {
            col_type = data_type::INTEGER;
        } else if (col_type_str == "float") {
            col_type = data_type::FLOAT;
        }
        column col(decl_vec[i].second, col_type);
        cols.push_back(col);
    }
    t.set_columns(cols);
    storage_engine::save_table(t);
    std::clog << "Table created" << std::endl;
    decl_vec.clear();
}
void UDBMS::Driver::drop_table()
{
    for (int i = 0; i < list_vec.size(); ++i) {
        storage_engine::delete_table(list_vec[i]);
    }
    std::clog << "Table dropped" << std::endl;
    list_vec.clear();
}
void UDBMS::Driver::show_create(std::string name)
{
    const table& t = storage_engine::load_table(name);
    std::cout << "*************************** 1. row ***************************" << std::endl;
    std::cout << "       Table: " << t.get_name() << std::endl;
    std::cout << "Create Table: " << storage_engine::show_create_table(t) << std::endl;
}
