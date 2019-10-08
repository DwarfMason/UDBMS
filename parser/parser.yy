%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines

%define api.namespace {UDBMS}
%define parser_class_name {DParse}
%error-verbose

%code requires{

    #include "parser/statement/CreateStatement.h"

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

void emit(char *s,...);

}

%define api.value.type variant
%define parse.assert

%token END

%token INTEGER
%token VARCHAR

%token SHOW
%token DROP
%token TABLE
%token CREATE

%token INSERT
%token SELECT
%token UPDATE
%token DELETE

%token INTO
%token VALUES
%token FROM
%token WHERE
%token SET
%token GROUP
%token ORDER
%token BY
%token HAVING

%token CONSTRAINT
%token UNIQUE
%token PRIMARY
%token KEY

%token NULL_y
%token NOT

%token<int> INTNUM
%token<float> APPROXNUM
%token<char*> STRING
%token<bool> BOOL
%token<char*> NAME


%locations

%type<std::vector<std::string>> select_expr_list name_list_expr

/* create table*/
%type<std::vector<CreateStatement::Constraint>> additional_constraints
%type<CreateStatement::Constraint> additional_params
%type<CreateStatement::Statement> create_params
%type<std::vector<CreateStatement::Column>> params_expr
%type<CreateStatement::Column> create_single_param
%type<std::vector<int>> param_flags
%type<int> create_flag
%type<CreateStatement::Constraint_expr> constraints_expr
%type<CreateStatement::Type> var_type
%start stmt_list
%%

stmt_list
    : stmt ';'
    | stmt '\n'
    | stmt_list stmt ';'
    ;

stmt
    : create_stmt
    ;


/*create table*/
create_stmt:
    CREATE TABLE NAME '('
    create_params ')'
    ;

create_params               /*Statement*/
    : params_expr                                {$$ = CreateStatement::Statement();$$.columns = $1;}
    | params_expr ',' additional_constraints     {$$ = CreateStatement::Statement();$$.columns = $1;$$.constraints = $3;}
    ;

params_expr                 /*std::vector<Column>*/
    : create_single_param                       {$$ = std::vector<CreateStatement::Column>();$$.push_back($1);}
    | params_expr ',' create_single_param       {$$ = $1; $$.push_back($3);}
    ;

create_single_param         /*Column*/
    : NAME var_type param_flags                 {$$ = CreateStatement::Column();$$.name = $1; $$.type = $2; $$.flags = $3;}
    | NAME var_type '(' INTNUM ')' param_flags  {$$ = CreateStatement::Column();$$.name = $1; $$.type = $2; $$.typeLen = $4; $$.flags = $6;}
    ;

param_flags                 /*std::vector<int>*/
    : create_flag               {$$ = std::vector<int>();$$.push_back($1);}
    | param_flags create_flag   {$$ = $1; $$.push_back($2);}

create_flag                 /*int*/
    :
    | UNIQUE            {$$ = CreateStatement::UNIQUE;}
    | PRIMARY KEY       {$$ = CreateStatement::PRIMARY;}
    | NOT NULL_y          {$$ = CreateStatement::NOT_NULL;}
    ;

additional_constraints      /*std::vector<Constraint>*/
    : additional_params                             {$$ = std::vector<CreateStatement::Constraint>();$$.push_back($1);}
    | additional_constraints ',' additional_params  {$$ = $1; $$.push_back($3);}
    ;

additional_params           /*Constraint*/
    : CONSTRAINT NAME constraints_expr          {$$ = CreateStatement::Constraint();$$.name = $2; $$.constraint = $3;}
    | constraints_expr                          {$$ = CreateStatement::Constraint();$$.constraint = $1;}
    ;

constraints_expr            /*Constraint_expr*/
    : PRIMARY KEY '(' name_list_expr ')'        {$$ = CreateStatement::Constraint_expr();$$.type = CreateStatement::Constraints::PRIMARY_KEY;$$.keys = $4;}
    | UNIQUE '(' name_list_expr ')'             {$$ = CreateStatement::Constraint_expr();$$.type = CreateStatement::Constraints::UNIQUE;$$.keys = $3;}
    ;

/*end create table*/

/*common*/
name_list_expr      /*std::vector<std::string>*/
    : NAME                      {$$ = std::vector<std::string>();$$.push_back($1);}
    | name_list_expr ',' NAME  {$$ = $1; $$.push_back($3);}
    ;

var_type    /*Type*/
    : INTEGER   {$$ = CreateStatement::Type::INTEGER;}
    | VARCHAR   {$$ = CreateStatement::Type::VARCHAR;}
    ;

%%


void UDBMS::DParse::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
   while(std::cin.get() != ';');
   driver.parse( std::cin );
}

void
emit(char *s, ...)
{

  va_list ap;

  printf("rpn: ");
  printf(s, ap);
  printf("\n");
}
