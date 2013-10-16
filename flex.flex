/* scanner for ET */

%{
/*left for includes*/

%}

/*definitions*/
%option c++
ID [a-zA-Z][a-zA-Z0-9]*
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
IDENTIFIER SAVE_TOKEN;return STRING;

%%

