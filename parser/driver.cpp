#include <fstream>
#include <logic/show_create.h>
#include <exceptions.h>

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
    /*TODO*/
    try {
        parse_helper(stream);
    }catch(sql_error e){
        std::cerr << e.error_code_ << ":" << e.msg_ <<"\n";
    }
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
    try {
        table tbl = API::create_table(stmt.tableName);
        auto cols = stmt.columns;
        std::map<std::string, column&> col_map;
        for (const CreateStatement::Column& col : cols)
        {
            auto type = static_cast<data_type>(col.type);
            column api_col(col.name, type);
            if (col.typeLen != -1) {
                api_col.set_size(col.typeLen);
            }
            if (!col.flags.empty())
            {
                constraints api_cts;
                for (int f : col.flags)
                {
                    switch (f) {
                        case CreateStatement::flag::UNIQUE:
                            api_cts.set_unique(true);
                            break;
                        case CreateStatement::flag::PRIMARY:
                            api_cts.set_not_null(true);
                            api_cts.set_unique(true);
                            break;
                        case CreateStatement::flag::NOT_NULL:
                            api_cts.set_not_null(true);
                            break;
                        default:
                            throw std::runtime_error("driver.cpp, Line " + std::to_string(__LINE__));
                    }
                }
                api_col.set_constraints(api_cts);
            }
            tbl.add_column(api_col);
            col_map.insert_or_assign(api_col.get_name(), api_col);
        }
        auto cts = stmt.constraints;

        for (const CreateStatement::Constraint& ct : cts) {
            // TODO Use name of the constraint
            constraints api_cts;
            switch (ct.constraint.type) {
                case CreateStatement::Constraints::PRIMARY_KEY:
                    api_cts.set_not_null(true);
                    api_cts.set_unique(true);
                    break;
                case CreateStatement::Constraints::UNIQUE:
                    api_cts.set_unique(true);
                    break;
                default:
                    throw std::runtime_error("driver.cpp, Line " + std::to_string(__LINE__));
            }
            for (const auto& col : ct.constraint.keys) {
                auto current_cts = col_map.at(col).get_constraints();
                current_cts.set_unique(current_cts.is_unique_key() || api_cts.is_unique_key());
                current_cts.set_not_null(current_cts.is_not_null() || api_cts.is_not_null());
                col_map.at(col).set_constraints(current_cts);
            }
        }
        API::commit_table(tbl);
        std::cout << "Table created." << std::endl;
    }
    catch (sql_error& e) {
        std::cerr << e.what() << std::endl;
    }
}
void UDBMS::Driver::drop_table(DropTableStatement::Statement stmt)
{
    try {
        for (const auto& table_name : stmt.keys)
        {
            const table& t = API::load_table(table_name);
            API::drop_table(t);
        }
    }
    catch (sql_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
void UDBMS::Driver::show_create(ShowCreateStatement::Statement stmt)
{
    try
    {
        const auto& t = API::load_table(stmt.name);
        Logic::show_create_table(t);
    }
    catch (sql_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

