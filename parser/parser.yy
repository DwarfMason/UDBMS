%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {UDBMS}
%define parser_class_name {DParse}
%error-verbose

%code requires{
   namespace UDBMS {
      class Driver;
      class Scanner;
   }
}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   #include "parser/driver.hpp"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token                      END
%token                      SHOW
%token                      DROP
%token                      CREATE
%token                      TABLE
%token                      CLOSE
%token                      CHAR
%token<std::string>             NAME
%token<std::string>             FLOATING
%token<std::string>          INT
%token<std::string>          FLOAT
%token<std::string>          DOUBLE
%token<std::string>          STRING

%token                 UNIQUE
%token                 CONSTRAINT
%token                 FOREIGN
%token                 KEY

%token<int>          NUMBER

%token          RBRACKET
%token          LBRACKET
%token          SEP
%token          DIV
%locations
%type<std::string> type
%%

commands: /* empty */
	|	commands command DIV
    ;

command:
		drop_table
	|	show_create
	|	create_table
	| 	END {YYABORT;}
	{
		exit(0);
	}
	;
//

create_table:
	CREATE TABLE NAME RBRACKET decl	constr LBRACKET
	{
		driver.create_table($3);
	}
	;

constr: /*empty*/
    | SEP CONSTRAINT NAME RBRACKET sublist LBRACKET
    | SEP FOREIGN KEY
    ;

type:
	FLOAT		{ $$ = $1; }
    |DOUBLE		{ $$ = $1; }
    |STRING		{ $$ = $1; }
	|INT		{ $$ = $1; }
	;

decl:
	| type NAME	flags	        {driver.decl_vec.emplace_back($1,$2);}
	| decl SEP type NAME flags	{driver.decl_vec.emplace_back($3,$4);}
	;

sublist:
        NAME
    |	sublist SEP NAME
    ;

flags
    :
    | UNIQUE    {driver.unique = true;}
    ;

drop_table:
	DROP TABLE list
	{
	 	driver.drop_table();
	}
	;

list:
		NAME				    {driver.list_vec.push_back($1);}
	|	list SEP NAME			{driver.list_vec.push_back($3);}
	;

show_create:
	SHOW CREATE TABLE NAME
	{
		driver.show_create($4);
	}
	;


%%


void UDBMS::DParse::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
   driver.parse( std::cin );
}
