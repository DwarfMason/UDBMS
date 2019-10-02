%{
#include <iostream>
#include <string>
#include <vector>
#include "../storage_engine.h"

int yylex();
int yyerror(std::string  s);

std::vector<std::string> list_vec;
std::vector<std::pair<std::string,std::string>> decl_vec;

void create_table(std::string name) {
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

    decl_vec.clear();
}

void drop_table() {
    for (int i = 0; i < list_vec.size(); ++i) {
        storage_engine::delete_table(list_vec[i]);
    }

    list_vec.clear();
}

void show_create(std::string name) {

}

%}
%defines
%error-verbose



%union
    {
            int number;
            char *string;
    }

%token <string> INT
%token <string> NAME

%token CLOSE
%token SHOW
%token DROP
%token CREATE
%token TABLE
%token QUIT
%token <string> FLOAT
%token <string> DOUBLE
%token <string> STRING

%token <number> NUMBER
%token FLOATING
%token END
%token RBRACKET
%token LBRACKET
%token SEP
%type <string> type
%%


commands: /* empty */
	|	commands command END
        ;

command:
		drop_table
	|	show_create
	|	create_table
	| 	QUIT
	{
		exit(0);
	}
	;
//

create_table:
	CREATE TABLE NAME RBRACKET decl	LBRACKET
	{
		create_table($3);
	}
type:
	FLOAT		{ $$ = $1; }
        |DOUBLE		{ $$ = $1; }
        |STRING		{ $$ = $1; }
	|INT		{ $$ = $1; }
	;

decl:
	| type NAME				{decl_vec.emplace_back($1,$2);}
	| decl SEP type NAME			{decl_vec.emplace_back($3,$4);}
	;


drop_table:
	DROP TABLE list
	{
	 	drop_table();
	}
	;

list:
		NAME				{list_vec.push_back($1);}
	|	list SEP NAME			{list_vec.push_back($3);std::cout<<$3<<"\n";}
	;

show_create:
	SHOW CREATE TABLE NAME
	{
		show_create($4);
	}
	;




%%


void set_input_string(const char* in);
void end_lexical_scan(void);


std::string parse_request(const char* in) {
  set_input_string(in);
  int res = yyparse();
  end_lexical_scan();
  return in;
}

int yyerror(std::string s){
    std::cerr << s  << ";\n";
    return 0;
}