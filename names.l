%{
    #include "commands.tab.h"

%}

%%
show                      {return SHOW;}
drop                      {return DROP;}
create                    {return CREATE;}
table                     {return TABLE;}
;                         {return END;}
close                     {return CLOSE;}

int                       {yylval.string = strdup(yytext); return INT;}
float                     {yylval.string = strdup(yytext); return FLOAT;}
double                    {yylval.string = strdup(yytext); return DOUBLE;}
string                    {yylval.string = strdup(yytext); return STRING;}
\(                        {return RBRACKET;}
\)                        {return LBRACKET;}
,                         {return SEP;}

[A-Za-z_][A-Za-z_0-9]*    {yylval.string = strdup(yytext);return NAME;}
-?[1-9][0-9]*             {yylval.number = atoi(yytext);return NUMBER;}
-?[1-9].[0-9][0-9]*       {yylval.number = atoi(yytext);return FLOATING;}

%%{
