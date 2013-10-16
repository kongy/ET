%{
	#include "AST.hpp"
	using namespace AST;
	
	LogicStatement *entireStatement; // Top of the LogicStatement
	
	
%}


%union {
	LogicStatement* logicStatement;
//	Truth* truth;
//	Falsity* falsity;
	Variable* variable;
	UnaryOpStatement* unaryOpStatement;
	NotStatement* notStatement;
	BinaryOpStatement* binaryOpStatement;
	AndStatement* andStatement;
	OrStatement* orStatement;
	IffStatement* iffStatement;
	ImpliesStatement* impliesStatement;
	FirstOrderStatement* firstOrderStatement;
	ForAllStatement* forAllStatement;
	ThereExistsStatement* thereExistsStatement;
	Parameters* parameters;
	PredicateSymbolStatement* predicateSymbolStatement;
	EqualityStatement* equalityStatement;
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
			   { $$ = new Truth(); }
			   | FALSITY
			   { $$ = new Falsity(); }
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
         { $$ = new Variable($1); }
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

ForAllStatement : FORALL IDENTIFIER LBRACKET LogicStatement RBRACKET
                { $$ = new ForAllStatement($2, $4); }
				;
				
EqualityStatement : IDENTIFIER EQUALS IDENTIFIER
                  { $$ = new EqualityStatement($1, $3); }
				  ;
				  
PredicateSymbolStatement : IDENTIFIER LBRACKET Params RBRACKET
                         { $$ = new PredicateSymbolStatement($1, $3); }
						 ;
						 
Params : IDENTIFIER
       { $$ = new Parameters(new Variable($1), NULL); }
       | IDENTIFIER COMMA Params
       { $$ = new Parameters(new Variable($1), $3); }
       ;
						 
ThereExistsStatement : THEREEXISTS IDENTIFIER LBRACKET LogicStatement RBRACKET
                     { $$ = new ThereExistsStatement($2, $4); }
					 ;
					 
UnaryOpStatement : NotStatement
                 { $$ = $1; }
				 ;

NotStatement : NOT LogicStatement
             { $$ = new NotStatement($2); }
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
             { $$ = new AndStatement($1, $3); }
			 ;

OrStatement : LogicStatement OR LogicStatement
            { $$ = new OrStatement($1, $3); }
			;
			
ImpliesStatement : LogicStatement IMPLIES LogicStatement
                 { $$ = new ImpliesStatement($1, $3); }
				 ;
				 
IffStatement : LogicStatement IFF LogicStatement
             { $$ = new IffStatement($1, $3); }
			 ;
