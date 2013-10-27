/* scanner for ET */

%{
/*left for includes*/
	#include "AST.hpp"
	#include "parser.hpp"
	#include <QString>
	#define SAVE_TOKEN (yylval.qString = new QString(yytext))
	#define TOKEN(t) (yylval.token = t)
%}

/*definitions*/
/* %option c++ */
%option yylineno

IDENTIFIER [a-zA-Z][a-zA-Z0-9]*
%%
"∧" return TOKEN(AND);
"↔" return TOKEN(IFF);
"∨" return TOKEN(OR);
"⊤" return TOKEN(TRUTH);
"⊥" return TOKEN(FALSITY);
"=" return TOKEN(EQUALS);
"→" return TOKEN(IMPLIES);
"¬" return TOKEN(NOT);
"∀" return TOKEN(FORALL);
"∃" return TOKEN(THEREEXISTS);
"(" return TOKEN(LBRACKET);
")" return TOKEN(RBRACKET);
"," return TOKEN(COMMA);
IDENTIFIER SAVE_TOKEN;return IDENTIFIER;

%%

