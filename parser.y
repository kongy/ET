%{
        #include "AST.hpp"
        #include <QString>
        #include <iostream>

        AST::LogicStatement *entireStatement; // Top of the LogicStatement

        extern int yylineno;
        void yyerror(const char *s) {
            std::cerr << "Line " << yylineno << ": " << s << std::endl;
        }
        extern int yylex(void);
%}

%error-verbose

%union {
        AST::LogicStatement* logicStatement;
//	AST::Truth* truth;
//	AST::Falsity* falsity;
        AST::Variable* variable;
        AST::UnaryOpStatement* unaryOpStatement;
        AST::NotStatement* notStatement;
        AST::BinaryOpStatement* binaryOpStatement;
        AST::AndStatement* andStatement;
        AST::OrStatement* orStatement;
        AST::IffStatement* iffStatement;
        AST::ImpliesStatement* impliesStatement;
        AST::FirstOrderStatement* firstOrderStatement;
        AST::ForAllStatement* forAllStatement;
        AST::ThereExistsStatement* thereExistsStatement;
        AST::Parameters* parameters;
        AST::PredicateSymbolStatement* predicateSymbolStatement;
        AST::EqualityStatement* equalityStatement;
	QString* qString;
	int token;
}

%token <token> LBRACKET
%token <token> RBRACKET
%token <token> FORALL
%token <token> THEREEXISTS
%token <token> EQUALS
%token <token> NOT
%token <token> AND
%token <token> OR
%token <token> IMPLIES
%token <token> IFF
%token <token> TRUTH
%token <token> FALSITY
%token <qString> IDENTIFIER
%token <token> COMMA

%type <logicStatement> MainStatement LogicStatement
%type <variable> Variable
//%type <truth> Truth
//%type <falsity> Falsity
%type <firstOrderStatement> FirstOrderStatement;
%type <unaryOpStatement> UnaryOpStatement;
%type <binaryOpStatement> BinaryOpStatement;
%type <forAllStatement> ForAllStatement;
%type <equalityStatement> EqualityStatement;
%type <predicateSymbolStatement> PredicateSymbolStatement;
%type <parameters> Params;
%type <thereExistsStatement> ThereExistsStatement;
%type <notStatement> NotStatement;
%type <andStatement> AndStatement;
%type <orStatement> OrStatement;
%type <impliesStatement> ImpliesStatement;
%type <iffStatement> IffStatement;

%left LBRACKET
%left RBRACKET
%left FORALL
%left THEREEXISTS
%left EQUALS
%left NOT
%left AND
%left OR
%left IMPLIES
%left IFF
%left TRUTH
%left FALSITY
%left IDENTIFIER
%left COMMA


%%

MainStatement : {entireStatement = NULL;}
              | LogicStatement
			  { entireStatement = $1; }

LogicStatement : TRUTH
                           { $$ = new AST::Truth(); }
			   | FALSITY
                           { $$ = new AST::Falsity(); }
               | Variable
               { $$ = $1; }
               | FirstOrderStatement
               { $$ = $1; }
               | UnaryOpStatement
               { $$ = $1; }
               | BinaryOpStatement
               { $$ = $1; }
               ;
               
Variable : IDENTIFIER
         { $$ = new AST::Variable($1); }
         ;
			   
FirstOrderStatement : ForAllStatement
                    { $$ = $1; }
                    | EqualityStatement
                    { $$ = $1; }
                    | PredicateSymbolStatement
                    { $$ = $1; }
                    | ThereExistsStatement
                    { $$ = $1; }
					;

ForAllStatement : FORALL Variable LBRACKET LogicStatement RBRACKET
                { $$ = new AST::ForAllStatement($2, $4); }
				;
				
EqualityStatement : Variable EQUALS Variable
                  { $$ = new AST::EqualityStatement($1, $3); }
				  ;
				  
PredicateSymbolStatement : Variable LBRACKET Params RBRACKET
                         { $$ = new AST::PredicateSymbolStatement($1, $3); }
						 ;
						 
Params : Variable
       { $$ = new AST::Parameters($1, NULL); }
       | Variable COMMA Params
       { $$ = new AST::Parameters($1, $3); }
       ;
						 
ThereExistsStatement : THEREEXISTS Variable LBRACKET LogicStatement RBRACKET
                     { $$ = new AST::ThereExistsStatement($2, $4); }
					 ;
					 
UnaryOpStatement : NotStatement
                 { $$ = $1; }
				 ;

NotStatement : NOT LogicStatement
             { $$ = new AST::NotStatement($2); }
			 ;
			 
BinaryOpStatement : AndStatement
                  { $$ = $1; }
				  | OrStatement
				  { $$ = $1; }
				  | ImpliesStatement
				  { $$ = $1; }
				  | IffStatement
				  { $$ = $1; }
				  ;
				  
AndStatement : LogicStatement AND LogicStatement
             { $$ = new AST::AndStatement($1, $3); }
			 ;

OrStatement : LogicStatement OR LogicStatement
            { $$ = new AST::OrStatement($1, $3); }
			;
			
ImpliesStatement : LogicStatement IMPLIES LogicStatement
                 { $$ = new AST::ImpliesStatement($1, $3); }
				 ;
				 
IffStatement : LogicStatement IFF LogicStatement
             { $$ = new AST::IffStatement($1, $3); }
			 ;
%%
int yywrap(void) {
    return 1;
}
