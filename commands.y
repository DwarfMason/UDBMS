%{
#include <iostream>
#include <string>
#include <vector>


extern "C"
{
	void yyerror(const char *str)
        {
                fprintf(stderr,"ошибка: %s\n",str);
        }

        int yyparse(void);
        int yylex(void);
        int yywrap()
        {
                return 1;
        }

}


std::vector<std::string> list_vec;
std::vector<std::pair<std::string,std::string>> decl_vec;
%}



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
		delete_table
	|	show_create
	|	create_table
	;
//

create_table:
	CREATE TABLE NAME RBRACKET decl	LBRACKET
	{
	std::cout << "_table created: " << "\n";
	for (int i = 0;i< decl_vec.size();++i){
		std::cout << decl_vec[i].first <<" "<< decl_vec[i].second <<"\n";
	    }
	    decl_vec.clear();
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


delete_table:
	DROP TABLE list
	{
	 std::cout << "_drop table: "<<"\n";
	 for (int i = 0; i < list_vec.size();++i){
		std::cout << list_vec[i] <<"\n";
	    }
	    list_vec.clear();
	}
	;

list:
		NAME				{list_vec.push_back($1);}
	|	list SEP NAME			{list_vec.push_back($3);std::cout<<$3<<"\n";}
	;

show_create:
	SHOW CREATE TABLE NAME
	;




%%


void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
