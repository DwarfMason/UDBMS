%{
#include <iostream>
#include <string>
#include <vector>


int yylex();
int yyerror(std::string  s);

std::vector<std::string> list_vec;
std::vector<std::pair<std::string,std::string>> decl_vec;

void create_table(){
	std::cout << "_table created: " << "\n";
		for (int i = 0;i< decl_vec.size();++i){
			std::cout << decl_vec[i].first <<" "<< decl_vec[i].second <<"\n";
		    }
		    decl_vec.clear();
}

void drop_table(){
	std::cout << "_drop table: "<<"\n";
        	 for (int i = 0; i < list_vec.size();++i){
        		std::cout << list_vec[i] <<"\n";
        	    }
        	    list_vec.clear();
	 }

void show_create(){

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
		create_table();
	}
type:
	FLOAT		{ $$ = $1; }
        |DOUBLE		{ $$ = $1; }
        |STRING		{ $$ = $1; }
	|INT		{ $$ = $1; }
	;

decl:
	type NAME				{decl_vec.emplace_back($1,$2);}
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
		show_create();
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
    std::cerr << s;
    return 0;
}