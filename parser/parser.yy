%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines

%define api.namespace {UDBMS}
%define parser_class_name {DParse}
%error-verbose

%code requires{

    #include "parser/statement/CreateStatement.h"
    #include "parser/statement/ShowCreateStatement.h"
    #include "parser/statement/DropTableStatement.h"
    #include "parser/statement/DeleteStatement.h"
    #include "parser/statement/InsertStatement.h"
    #include "parser/statement/SelectStatement.h"
    #include "parser/statement/UpdateStatement.h"

    #include "exceptions.h"

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
%token CHAR
%token FLOAT

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

%token AND
%token OR

%token<std::string> INTNUM
%token<std::string> APPROXNUM
%token<std::string> STRING
%token<std::string> BOOL
%token<std::string> NAME


%locations

%type<std::vector<std::string>> name_list_expr

/* create Table*/
%type<std::vector<CreateStatement::Constraint>> additional_constraints
%type<CreateStatement::Constraint> additional_params
%type<CreateStatement::Statement> create_params
%type<std::vector<CreateStatement::Column>> params_expr
%type<CreateStatement::Column> create_single_param
%type<std::vector<int>> param_flags
%type<int> create_flag
%type<CreateStatement::Constraint_expr> constraints_expr
%type<CreateStatement::Type> var_type
/* show create */
%type<ShowCreateStatement::Statement> show_create
/* drop Table */
%type<DropTableStatement::Statement> drop_table_stmt

/*insert*/
%type<InsertStatement::Statement> insert_stmt
/*delete*/
%type<DeleteStatement::Statement> delete_stmt
/*update*/
%type<UpdateStatement::Statement> update_stmt
/*select*/
%type<SelectStatement::Statement> select_stmt
%type<std::vector<std::string>> selector

/*expr*/ /*TODO*/
%type<std::pair<std::string,std::string>> expr

%type<std::string> value
%type<std::vector<std::string>> values_list_expr
%start stmt_list
%%

stmt_list
    :
    | stmt ';'
    | stmt_list stmt ';'
    ;

stmt
    : create_stmt
    | show_create
    | drop_table_stmt
    | select_stmt
    | update_stmt
    | insert_stmt
    | delete_stmt
    | END {exit(0);}
    ;


/*create Table*/
create_stmt:
    CREATE TABLE NAME '(' create_params ')' {$5.tableName = $3;driver.create_table($5);}
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
    : NAME var_type                             {$$ = CreateStatement::Column();$$.name = $1;$$.type = $2;}
    | NAME var_type '(' INTNUM ')'              {$$ = CreateStatement::Column();$$.name = $1;$$.type = $2; $$.typeLen = atoi($4.c_str());}
    | NAME var_type param_flags                 {$$ = CreateStatement::Column();$$.name = $1; $$.type = $2; $$.flags = $3;}
    | NAME var_type '(' INTNUM ')' param_flags  {$$ = CreateStatement::Column();$$.name = $1; $$.type = $2; $$.typeLen = atoi($4.c_str()); $$.flags = $6;}
    ;

param_flags                 /*std::vector<int>*/
    : create_flag               {$$ = std::vector<int>();$$.push_back($1);}
    | param_flags create_flag   {$$ = $1; $$.push_back($2);}

create_flag                 /*int*/
    : UNIQUE            {$$ = CreateStatement::UNIQUE;}
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

/*end create Table*/

/*show create*/
show_create:
	SHOW CREATE TABLE NAME
	{
	    $$ = ShowCreateStatement::Statement();
	    $$.name = $4;
		driver.show_create($$);
	}
	;
/*end show create*/

/*drop Table*/
drop_table_stmt:
	DROP TABLE name_list_expr
	{
	    $$ = DropTableStatement::Statement();
        $$.keys = $3;
	 	driver.drop_table($$);
	}
	;
/*end drop Table */





/*insert*/
insert_stmt:        /*InsertStatement::Statement*/
    INSERT INTO NAME '(' name_list_expr ')'
    VALUES '(' values_list_expr ')'   {
        $$ = InsertStatement::Statement();
        $$.name = $3;
        $$.cols = $5;
        $$.value = $9;
        driver.insert($$);
    }
    ;
/*end insert*/

/*update stmt*/
update_stmt:        /*UpdateStatement::Statement*/
    UPDATE NAME SET NAME '=' value WHERE expr  {
        $$ = UpdateStatement::Statement();
        $$.tableToUpdate = $2;
        $$.columnName = $4;
        $$.newValue = $6;
        $$.expr = $8;
        driver.update($$);
    }
    ;

/*end update stmt*/

/*delete stmt*/
delete_stmt:            /*DeleteStatement::Statement*/
    DELETE FROM NAME
      WHERE expr              {
        $$ = DeleteStatement::Statement();
        $$.name = $3;
        $$.expr = $5;
        driver.delete_stmt($$);
    }
    ;

/*end delete stmt*/

/*select expr*/
select_stmt             /*SelectStatement::Statement*/
    : SELECT selector FROM NAME WHERE expr    {
        $$ = SelectStatement::Statement();
        $$.name = $4;
        $$.selector = $2;
        $$.expr = $6;
        driver.select($$);
    }
    | SELECT selector FROM NAME   {
              $$ = SelectStatement::Statement();
              $$.name = $4;
              $$.selector = $2;
              driver.select($$);
          }
    ;

selector    /*std::vector<std::string> */
    : '*'                       {$$ = std::vector<std::string>();$$.push_back("*");}
    | name_list_expr            {$$ = $1;}

/*end select expr*/

/*expr */ /*TODO*/
expr
    : value '=' value {$$ = std::pair<std::string,std::string>($1,$3);}
/*    | value '-' value
    | value '*' value
    | value '/' value
    | value '>' value
    | value '>=' value
    | value '<=' value
    | value '>' value
    | value '<' value
    | value AND value
    | value OR value */
    ;


/*common*/
name_list_expr      /*std::vector<std::string>*/
    : NAME                      {$$ = std::vector<std::string>();$$.push_back($1);}
    | name_list_expr ',' NAME  {$$ = $1; $$.push_back($3);}
    ;

var_type    /*Type*/
    : INTEGER   {$$ = CreateStatement::Type::INTEGER;}
    | CHAR      {$$ = CreateStatement::Type::CHAR;}
    | FLOAT     {$$ = CreateStatement::Type::FLOAT;}
    ;

value       /*std::string*/
    : INTNUM       {$$ = $1;}
    | APPROXNUM    {$$ = $1;}
    | STRING       {$$ = $1;}
    | BOOL         {$$ = $1;}
    | NAME         {$$ = $1;}

values_list_expr        /*std::vector<std::string>*/
    : value                         {$$ = std::vector<std::string>();$$.push_back($1);}
    | values_list_expr ',' value    {$$ = $1;$$.push_back($3);}
%%


void UDBMS::DParse::error( const location_type &l, const std::string &err_message )
{
   throw parser_error(err_message);
}

void
emit(char *s, ...)
{

  va_list ap;

  printf("rpn: ");
  printf(s, ap);
  printf("\n");
}
