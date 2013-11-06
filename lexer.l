/* scanner for ET */

%{
/*left for includes*/
        #include "AST.hpp"
        #include "parser.hpp"
        #define SAVE_TOKEN (yylval.qString = new QString(yytext))
        #define TOKEN(t) (yylval.token = t)

        #define YY_NO_INPUT

        extern QString lexerErrorMessage;
%}

/*definitions*/
/* %option c++ */
%option yylineno

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
[a-zA-Z][a-zA-Z0-9]* SAVE_TOKEN; return IDENTIFIER;
. lexerErrorMessage = QString("Unknown token: %1").arg(yytext); yyterminate();

%%

