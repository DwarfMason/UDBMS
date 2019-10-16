%option yylineno yywrap case-insensitive
%option yyclass="UDBMS::Scanner"
%option c++
%option stack
%{
#include <string>
#include <stdarg.h>

#include "parser/statement/CreateStatement.h"

#include "parser/scanner.hpp"
#undef  YY_DECL
#define YY_DECL int UDBMS::Scanner::yylex(UDBMS::DParse::semantic_type * const lval, UDBMS::DParse::location_type *loc)

using token = UDBMS::DParse::token;


#define YY_NO_UNISTD_H
%}


%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}


quit              { return token::END; }


INTEGER     { return token::INTEGER;    }
VARCHAR     { return token::VARCHAR;    }
CHAR        { return token::CHAR;       }
FLOAT       { return token::FLOAT;      }


SHOW        { return token::SHOW;       }
DROP        { return token::DROP;       }
TABLE       { return token::TABLE;      }
CREATE      { return token::CREATE;     }

INSERT      { return token::INSERT;     }
SELECT      { return token::SELECT;     }
UPDATE      { return token::UPDATE;     }
DELETE      { return token::DELETE;     }

INTO        { return token::INTO;       }
VALUES      { return token::VALUES;     }
FROM        { return token::FROM;       }
WHERE       { return token::WHERE;      }
SET         { return token::SET;        }
GROUP       { return token::GROUP;      }
ORDER       { return token::ORDER;      }
BY          { return token::BY;         }
HAVING      { return token::HAVING;     }

CONSTRAINT  { return token::CONSTRAINT; }
UNIQUE      { return token::UNIQUE;     }
PRIMARY     { return token::PRIMARY;    }
KEY         { return token::KEY;        }
NOT         { return token::NOT;        }
NULL        { return token::NULL_y;     }

(\/\*)[^(\*\/)]*(\*\/) {}

-?[0-9]+    { yylval->build<std::string>(yytext); return token::INTNUM; }

-?[0-9]+"."[0-9]*       |
-?"."[0-9]+     |
-?[0-9]+E[-+]?[0-9]+    |
-?[0-9]+"."[0-9]*E[-+]?[0-9]+ |
-?"."[0-9]+E[-+]?[0-9]+ { yylval->build<std::string>(yytext); return token::APPROXNUM; }


'(\\.|''|[^'\n])*'   |
\"(\\.|\"\"|[^"\n])*\"  { yylval->build<std::string>(yytext); return token::STRING; }


[-+&~|^/%*(),.;!]   { return yytext[0]; }
=                   { return yytext[0]; }

AND     {return token::AND;}
OR      {return token::OR;}

TRUE    { yylval->build<std::string>(yytext); return token::BOOL; }
FALSE   { yylval->build<std::string>(yytext); return token::BOOL; }

[A-Za-z][A-Za-z0-9_]* { yylval->build<std::string>(yytext); return token::NAME; }
`[^`/\\.\n]+`         { std::string temp(yytext);
    yylval->build<std::string>(temp.substr(1,temp.size()-2));
    return token::NAME;
}

%%

int yyFlexLexer::yywrap()
{
    return 1;
}