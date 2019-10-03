%{
#include <string>


#include "parser/scanner.hpp"
#undef  YY_DECL
#define YY_DECL int UDBMS::Scanner::yylex( UDBMS::DParse::semantic_type * const lval, UDBMS::DParse::location_type *loc )

using token = UDBMS::DParse::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option yyclass="UDBMS::Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

db_quit\(\);              {return token::END ;}

show                      {return token::SHOW;}
drop                      {return token::DROP;}
create                    {return token::CREATE;}
table                     {return token::TABLE;}
close                     {return token::CLOSE;}
unique                    {return token::UNIQUE;}
constraint                {return token::CONSTRAINT;}
int                       {
        yylval->build<std::string>(yytext);
        return token::INT;
        }
float                     {
        yylval->build<std::string>(yytext);
        return token::FLOAT;
        }
double                    {
        yylval->build<std::string>(yytext);
        return token::DOUBLE;
        }
string                    {
        yylval->build<std::string>(yytext);
        return token::STRING;
        }

(^`)[A-Za-z_][A-Za-z_0-9]*(`$)  {
        yylval->build<std::string>(yytext);
        return token::NAME;
        }
[A-Za-z_][A-Za-z_0-9]*  {
        yylval->build<std::string>(yytext);
        return token::NAME;
        }

-?[1-9][0-9]*             {
        yylval->build<int>() = atoi(yytext);
        return token::NUMBER;
        }

\(                        {return token::RBRACKET;}
\)                        {return token::LBRACKET;}
,                         {return token::SEP;}
;                         {return token::DIV;}

%%


