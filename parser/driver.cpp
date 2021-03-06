#include <fstream>
#include <logic/show_create.h>
#include <exceptions.h>
#include <api/TableFileWorker.h>
#include <api/data_wrap/BaseDataWrapper.h>
#include <iomanip>
#include <type_traits>

#include "driver.hpp"
#include "statement/BaseStatement.h"
#include "ext/libfort/lib/fort.hpp"
UDBMS::Driver::~Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

int getPosByName(const std::vector<Column> &cols,const std::string &name){
    for (int i = 0; i < cols.size(); ++i) {
        if  (cols.at(i).get_name() == name) {
            return i;
        }
    }
    return -1;
}

std::map<std::string,int> getTableNames(TableMetadata table){
    std::map<std::string,int> res;
    auto colums = table.get_columns();
    for(int i = 0;i < colums.size();++i){
        res.insert(std::pair(colums[i].get_name(),i));
    }
    return res;
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

        auto cols = stmt.columns;
        std::map<std::string, Column&> col_map;
        std::vector<Column> cols_vec;
        for (const CreateStatement::Column& col : cols)
        {
            auto type = static_cast<DataType>(col.type);
            Column api_col(col.name, type);
            if (col.typeLen != -1) {
                api_col.set_logic_size(col.typeLen);
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
            cols_vec.push_back(api_col);
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
        TableMetadata tbl = TableFileWorker::create_table(stmt.tableName, cols_vec);
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
            const TableMetadata& t = TableFileWorker::load_table(table_name);
            TableFileWorker::drop_table(t);
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
        const auto& t = TableFileWorker::load_table(stmt.name);
        Logic::show_create_table(t);
    }
    catch (sql_error& e)
    {
        std::cerr << e.msg() << std::endl;
    }
}

void UDBMS::Driver::delete_stmt(DeleteStatement::Statement stmt)
{
    auto table = TableFileWorker::load_table(stmt.name);
    BaseDataWrapper* wrapper = TableFileWorker::load_table_data(table);
    auto idMap = getTableNames(table);
    auto colums = table.get_columns();
    while (true) {
        try {
            Row row = wrapper->get_current_row();
            bool flag = false;
            try {
                if (std::any_cast<int>(stmt.expr.second) == row.get_at(idMap[stmt.expr.first]).get_value<int>())
                    flag = true;
            } catch (std::bad_any_cast &e){
                try {
                    if (std::any_cast<float>(stmt.expr.second) == row.get_at(idMap[stmt.expr.first]).get_value<float>())
                        flag = true;
                } catch (std::bad_any_cast &e){
                    try {
                        if (std::any_cast<std::string>(stmt.expr.second) == row.get_at(idMap[stmt.expr.first]).get_value<std::string>())
                            flag = true;
                    } catch (...){
                        throw custom_exception(666,"invalid type");
                    }

                }

            }

            if (flag){
                wrapper->next_row();
                wrapper->delete_current_row();
            }else
            wrapper->next_row();
        }
        catch (cursor_eof_error& err) {
            break;
        }
    }
    std::cout << "Deleted" << std::endl;
    delete wrapper;
}

/*TODO constraint error*/
void UDBMS::Driver::update(UpdateStatement::Statement stmt)
{

    /*std::cout << "Table to update: " << stmt.tableToUpdate << '\n';
    std::cout << "new value: " << stmt.newValue << '\n';
    std::cout << "Column name" << stmt.columnName << '\n';
    std::cout << "where: " << stmt.expr.first << " = " << stmt.expr.second << '\n';*/

    auto table = TableFileWorker::load_table(stmt.tableToUpdate);
    BaseDataWrapper* wrapper = TableFileWorker::load_table_data(table);

    std::vector<Column> columns = table.get_columns();
    int pos = getPosByName(columns,stmt.columnName);
    auto idMap = getTableNames(table);
    while (true) {
        try {
            Row row = wrapper->get_current_row();
            bool flag = false;
            try {
                if (std::any_cast<int>(stmt.expr.second) == row.get_at(idMap[stmt.expr.first]).get_value<int>())
                    flag = true;
            } catch (std::bad_any_cast &e){
                try {
                    if (std::any_cast<float>(stmt.expr.second) == row.get_at(idMap[stmt.expr.first]).get_value<float>())
                        flag = true;
                } catch (std::bad_any_cast &e){
                    try {
                        if (std::any_cast<std::string>(stmt.expr.second) == row.get_at(idMap[stmt.expr.first]).get_value<std::string>())
                            flag = true;
                    } catch (...){
                        throw custom_exception(666,"invalid type");
                    }

                }

            }

            if (flag){
                std::map<std::string,std::any> value = {{stmt.columnName,stmt.newValue}};
                wrapper->update_current_row(value);
            }
            wrapper->next_row();
        }
        catch (cursor_eof_error& err) {
            break;
        }
    }
    std::cout << "Updated" << std::endl;
    delete wrapper;
}



void UDBMS::Driver::select(SelectStatement::Statement stmt)
{
    TableMetadata table = TableFileWorker::load_table(stmt.name);
    BaseDataWrapper* wrapper = TableFileWorker::load_table_data(table);
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
                    throw sql_error(303,"No such column!");
                }
            }
        } else {
            /*if selector is '*'*/
            for (int i = 0; i < columns.size(); ++i) {
                selectedCols.push_back(i);
            }
        }

        while (true){
            try {
                Row row = wrapper->get_current_row();
                std::vector<std::string> tempData;
                auto idMap = getTableNames(table);
                bool flag = false;
                if (stmt.useExpr) {
                    try {
                        if (std::any_cast<int>(stmt.expr.second) == row.get_at(idMap[stmt.expr.first]).get_value<int>())
                            flag = true;
                    } catch (std::bad_any_cast &e) {
                        try {
                            if (std::any_cast<float>(stmt.expr.second) ==
                                row.get_at(idMap[stmt.expr.first]).get_value<float>())
                                flag = true;
                        } catch (std::bad_any_cast &e) {
                            try {
                                if (std::any_cast<std::string>(stmt.expr.second) ==
                                    row.get_at(idMap[stmt.expr.first]).get_value<std::string>())
                                    flag = true;
                            } catch (...) {
                                throw custom_exception(666, "invalid type");
                            }

                        }

                    }
            } else
                flag = true;
                    if ((flag || !stmt.useExpr) && !wrapper->get_current_row().is_deleted()) {
                        for (int selectedCol : selectedCols) {
                            if (columns[selectedCol].get_type() == DataType::FLOAT)
                                tempData.push_back(std::to_string(
                                        row.get_at(selectedCol).get_value<cell_type_v<DataType::FLOAT>>()
                                ));
                            if (columns[selectedCol].get_type() == DataType::CHAR)
                                tempData.push_back(
                                        row.get_at(selectedCol).get_value<cell_type_v<DataType::CHAR>>()
                                );
                            if (columns[selectedCol].get_type() == DataType::INTEGER)
                                tempData.push_back(std::to_string(
                                        row.get_at(selectedCol).get_value<cell_type_v<DataType::INTEGER>>()
                                ));
                        }
                        data.push_back(tempData);
                    }
                    wrapper->next_row();

            } catch (cursor_eof_error& err){
                break;
            }
        }


    /*output*/
        fort::char_table fTable;
        fTable << fort::header;
        for (int k = 0; k < selectedCols.size(); ++k) {
            fTable << table.get_columns()[selectedCols[k]].get_name();
        }
        fTable << fort::endr;
        for (int k = 0; k < data.size(); ++k) {
            for (int j = 0; j < data[k].size(); ++j) {
                fTable << data[k][j];
            }
            fTable << fort::endr;
        }
        std::cout << fTable.to_string();

    delete wrapper;
}

void UDBMS::Driver::insert(InsertStatement::Statement stmt)
{
    /*std::cout << "name: " << stmt.name << '\n';
    for (const auto & col : stmt.cols) {
        std::cout << "Column to insert:" << col <<'\n';
    }
    for (const auto & i : stmt.value) {
        std::cout << "value to insert:" << i <<'\n';
    }*/

    TableMetadata tbl = TableFileWorker::load_table(stmt.name);
    BaseDataWrapper* wrapper = TableFileWorker::load_table_data(tbl);
    std::vector<Column> columns = tbl.get_columns();
    std::map<std::string, std::any> mp;

    for (int i = 0; i < stmt.cols.size(); ++i) {
        int columnPos = getPosByName(columns,stmt.cols[i]);
        if (columnPos != -1){
            //mp.insert_or_assign(stmt.cols[i], ValueManager::createPointer(stmt.value[i],columns[columnPos].get_type()));
            //TODO move data create somewhere
            mp.insert(std::pair(stmt.cols[i], stmt.value[i]));
            std::cout << "Inserted.\n";
        } else {
            throw sql_error(303,"no such column");
        }
    }
    /*TODO check it on valid engine*/
    std::vector<Cell> empty_cells;
    for (const auto& col : columns)
    {
        empty_cells.emplace_back(col.get_type(), col.get_phys_size());
    }
    Row new_row(empty_cells);

    for (const auto& kv : mp)
    {
        size_t index = tbl.get_index_by_name(kv.first);
        new_row.set_at(index, kv.second);
    }
    wrapper->insert_row(new_row);
    delete wrapper;
}

