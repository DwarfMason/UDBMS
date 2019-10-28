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

void UDBMS::Driver::parse(std::stringstream &stream )
{
    /*TODO*/
    try {
        parse_helper(stream);    }
    catch(sql_error & e)
    {
        std::cerr << e.error_code_ << ":" << e.msg_ << "\n";
        /*char a = stream.get();
        while(a != '\n') a = stream.get();*/
        //parse(stream);
    }

}


void UDBMS::Driver::parse_helper(std::stringstream &stream )
{
      delete(scanner);
      scanner = new UDBMS::Scanner(&stream );
      scanner->set_debug(true);
      parser = new UDBMS::DParse((*scanner) /* scanner */,
                                    (*this) /* driver */ );
      parser->parse();
}

void UDBMS::Driver::create_table(CreateStatement::Statement stmt)
{
    try {
        Table tbl = API::create_table(stmt.tableName);
        auto cols = stmt.columns;
        std::map<std::string, Column&> col_map;
        for (const CreateStatement::Column& col : cols)
        {
            auto type = static_cast<data_type>(col.type);
            Column api_col(col.name, type);
            if (col.typeLen != -1) {
                api_col.set_size(col.typeLen);
            }
            if (!col.flags.empty())
            {
                Constraints api_cts;
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
            Constraints api_cts;
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
        std::cerr << e.msg() << std::endl;
    }
}
void UDBMS::Driver::drop_table(DropTableStatement::Statement stmt)
{
    try {
        for (const auto& table_name : stmt.keys)
        {
            const Table& t = API::load_table(table_name);
            API::drop_table(t);
            std::cout << "Table dropped." << std::endl;
        }
    }
    catch (sql_error& e)
    {
        std::cerr << e.msg() << std::endl;
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
        std::cerr << e.msg() << std::endl;
    }
}
void UDBMS::Driver::delete_stmt(DeleteStatement::Statement stmt)
{
    std::cout << stmt.name << "\n";
    std::cout << "where: " << stmt.expr.first << " = " << stmt.expr.second << '\n';
    auto table = API::load_table(stmt.name);

    table.load_data();
    Cursor curs(table);
    curs.next();
    while (true) {
        try {
            curs.next();
            if (false) /* TODO expr result*/
                curs.remove();
        }
        catch (cursor_eof_error) {
            break;
        }
    }
    API::commit_table(table);
}

int getPosByName(const std::vector<Column> &cols,const std::string &name){
    for (int i = 0; i < cols.size(); ++i) {
        if  (cols.at(i).get_name() == name) {
            return i;
        }
    }
    return -1;
}

/*TODO constraint error*/
void UDBMS::Driver::update(UpdateStatement::Statement stmt)
{

    std::cout << "Table to update: " << stmt.tableToUpdate << '\n';
    std::cout << "new value: " << stmt.newValue << '\n';
    std::cout << "Column name" << stmt.columnName << '\n';
    std::cout << "where: " << stmt.expr.first << " = " << stmt.expr.second << '\n';


    auto table = API::load_table(stmt.tableToUpdate);
    table.load_data();
    Cursor curs(table);
    std::vector<Column> columns = table.get_columns();
    int pos = getPosByName(columns,stmt.columnName);
    while (true) {
        try {
            Row row = curs.next();
            if (false) /* TODO expr result*/
                memcpy(row.at(pos),
                    ValueManager::createPointer(stmt.newValue,columns[pos].get_type()),
                    columns[pos].get_size());
        }
        catch (cursor_eof_error) {
            break;
        }
    }
    API::commit_table(table);
}



void UDBMS::Driver::select(SelectStatement::Statement stmt)
{

    Table table = API::load_table(stmt.name);
    table.load_data();
    std::vector<Column> columns = table.get_columns();

    std::vector<std::vector<std::string>> data;

    /*TODO make it readable*/

        std::vector<int> selectedCols;
        int pose;
        if  (stmt.selector[0] != "*"){
            for (const auto & j : stmt.selector) {
                pose = getPosByName(columns,j);
                if(pose != -1){
                    selectedCols.push_back(pose);
                }else{
                    throw sql_error(303,"no such column");
                }
            }
        } else {
            /*if selector is '*'*/
            for (int i = 0; i < columns.size(); ++i) {
                selectedCols.push_back(i);
            }
        }

        Cursor curs(table);
        while (true){
            try {
                Row row = curs.next();
                std::vector<std::string> tempData;
                for (int selectedCol : selectedCols) {
                    if (columns[selectedCol].get_type() == data_type::FLOAT)
                        tempData.push_back(std::to_string(ValueManager::getFloat(row.at(selectedCol))));
                    if (columns[selectedCol].get_type() == data_type::CHAR)
                        tempData.push_back(std::string(ValueManager::getChar(row.at(selectedCol),columns[selectedCol].get_size())));
                    if (columns[selectedCol].get_type() == data_type::INTEGER)
                        tempData.push_back(std::to_string(ValueManager::getInt(row.at(selectedCol))));
                }
                data.push_back(tempData);
            }catch (cursor_eof_error){
                break;
            }
        }


    /*output*/
    for (int k = 0; k < data.size(); ++k) {
        for (int j = 0; j < data[k].size(); ++j) {
            std::cout << std::setw(20) << std::right << data[k][j];
        }
        std::cout << std::endl;
    }
}



void UDBMS::Driver::insert(InsertStatement::Statement stmt)
{
    std::cout << "name: " << stmt.name << '\n';
    for (const auto & col : stmt.cols) {
        std::cout << "Column to insert:" << col <<'\n';
    }
    for (const auto & i : stmt.value) {
        std::cout << "value to insert:" << i <<'\n';
    }

    Table tbl = API::load_table(stmt.name);
    tbl.load_data();
    std::vector<Column> columns = tbl.get_columns();
    std::map<std::string,void*> mp;

    for (int i = 0; i < stmt.cols.size(); ++i) {
        int columnPos = getPosByName(columns,stmt.cols[i]);
        if (columnPos != -1){
            mp.insert_or_assign(stmt.cols[i],ValueManager::createPointer(stmt.value[i],columns[columnPos].get_type()));
        } else {
            throw sql_error(303,"no such column");
        }
    }
    /*TODO check it on valid engine*/
    tbl.insert_row(mp);
    API::commit_table(tbl);
}




void *UDBMS::Driver::ValueManager::createFoat(std::string val)
{
    float *p;
    try{
        p = new float(std::stof(strdup(val.c_str())));
    } catch (...) {
        throw custom_exception(304,"bad value type");
    }

    return static_cast<void*>(p);
}
void *UDBMS::Driver::ValueManager::createChar(std::string val)
{
    char *p;
    try{
        p = strdup(val.c_str());
    } catch (...) {
        throw custom_exception(304,"bad value type");
    }

    return static_cast<void*>(p);
}
void *UDBMS::Driver::ValueManager::createInt(std::string val)
{
    int *p;
    try{
        p = new int(std::stoi(strdup(val.c_str())));
    } catch (...) {
        throw custom_exception(304,"bad value type");
    }

    return static_cast<void*>(p);
}
void *UDBMS::Driver::ValueManager::createPointer(std::string str, data_type type)
{
    if(data_type::INTEGER == type){
        return createInt(str);
    }
    if(data_type::FLOAT == type){
        return createFoat(str);
    }
    if(data_type::CHAR == type){
        return createChar(str);
    }
}
