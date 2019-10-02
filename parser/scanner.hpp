#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif


#include "parser.tab.hh"
#include "location.hh"

namespace UDBMS{

class Scanner : public yyFlexLexer{
public:
   
   Scanner(std::istream *in) : yyFlexLexer(in)
   {
   };
   virtual ~Scanner() {
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex(UDBMS::DParse::semantic_type * const lval,
             UDBMS::DParse::location_type *location );
   // YY_DECL defined in lexer.l
   // Method body created by flex in mc_lexer.yy.cc

private:
   /* yyval ptr */
   UDBMS::DParse::semantic_type *yylval = nullptr;
};

} /* end namespace UDBMS */

#endif /* END __SCANNER_HPP__ */
