%{
    #include "commands.h"

%}

%option noinput noyywrap

%%
show                      {return SHOW;}
drop                      {return DROP;}
create                    {return CREATE;}
table                     {return TABLE;}
;                         {return END;}
close                     {return CLOSE;}
quit                      {return QUIT;}

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

%%

void set_input_string(const char* in) {
  yy_scan_string(in);
}

void end_lexical_scan(void) {
  yy_delete_buffer(YY_CURRENT_BUFFER);
}
