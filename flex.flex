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
"⊤" return TOKEN(TRUE);
"⊥" return TOKEN(FALSE);
"=" return TOKEN(EQUAL);
"→" return TOKEN(IMPLY);
"≡" return TOKEN(EQUALCE);
"¬" return TOKEN(NOT);
"∀" return TOKEN(FORALL);
"∃" return TOKEN(EXIST);
"(" return TOKEN(LBRAC);
")" return TOKEN(RBRAC);
ID SAVE_TOKEN;return STRING;
%%

