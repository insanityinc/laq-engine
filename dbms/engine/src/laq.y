%{
#include <string>
#include <iostream>

extern int yylex();
extern int yylineno;

void yyerror(const char *);

%}

%union{
	std::string *str;
}


%token <str> FILTER MAP DOT KRAO HADAMARD
%token <str> COUNT SUM AVG MIN MAX
%token <str> POWER EXP SQRT MATCH REGEXP
%token <str> IDENTIFIER INT FLOAT STRING DATE
%token <str> OR AND EQ NE LE GE LEFT RIGHT NOT


%type <str> expression logical_and_expression inclusive_or_expression exclusive_or_expression
%type <str> and_expression equality_expression relational_expression shift_expression
%type <str> additive_expression multiplicative_expression unary_expression primary_expression
%type <str> function data_type identifier product bang

%start laquery

%%

laquery
	: statement													{ ; }
	| laquery statement											{ ; }
	;

statement
	: IDENTIFIER '=' product '(' identifier ',' identifier ')'	{ ; }
	| IDENTIFIER '=' bang '(' identifier ')'					{ ; }
	| IDENTIFIER '=' FILTER '(' expression ')'					{ ; }
	| IDENTIFIER '=' MAP '(' inclusive_or_expression ')'		{ ; }
	;

identifier
	: IDENTIFIER												//{ $$ = $1;}
	| IDENTIFIER '.' IDENTIFIER									//{ $$ = *$1 + "." + $3; }
	;

product
	: DOT														//{ $$ = std::string("dot"); }
	| KRAO														//{ $$ = std::string("krao"); }
	| HADAMARD													//{ $$ = std::string("hadamard"); }
	;

bang
	: COUNT														//{ $$ = std::string("count"); }
	| SUM														//{ $$ = std::string("sum"); }
	| AVG														//{ $$ = std::string("avg"); }
	| MIN														//{ $$ = std::string("min"); }
	| MAX														//{ $$ = std::string("max"); }
	;

expression
	: logical_and_expression									//{ $$ = $1; }
	| expression OR logical_and_expression						//{ $$ = $1) + "||" + $3; }
	;

logical_and_expression
	: inclusive_or_expression									//{ $$ = $1; }
	| logical_and_expression AND inclusive_or_expression		//{ $$ = $1) + "&&" + $3; }
	;

inclusive_or_expression
	: exclusive_or_expression									//{ $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression		//{ $$ = $1) + "|" + $3; }
	;

exclusive_or_expression
	: and_expression											//{ $$ = $1; }
	| exclusive_or_expression '^' and_expression				//{ $$ = $1) + "^" + $3; }
	;

and_expression
	: equality_expression										//{ $$ = $1; }
	| and_expression '&' equality_expression					//{ $$ = $1) + "&" + $3; }
	;

equality_expression
	: relational_expression										//{ $$ = $1; }
	| equality_expression EQ relational_expression				//{ $$ = $1) + "==" + $3; }
	| equality_expression NE relational_expression				//{ $$ = $1) + "!=" + $3; }
	;

relational_expression
	: shift_expression											//{ $$ = $1; }
	| relational_expression '<' shift_expression				//{ $$ = $1) + "<" + $3; }
	| relational_expression '>' shift_expression				//{ $$ = $1) + ">" + $3; }
	| relational_expression LE shift_expression					//{ $$ = $1) + "<=" + $3; }
	| relational_expression GE shift_expression					//{ $$ = $1) + ">=" + $3; }
	;

shift_expression
	: additive_expression										//{ $$ = $1; }
	| shift_expression LEFT additive_expression					//{ $$ = $1) + "<<" + $3; }
	| shift_expression RIGHT additive_expression				//{ $$ = $1) + ">>" + $3; }
	;

additive_expression
	: multiplicative_expression									//{ $$ = $1; }
	| additive_expression '+' multiplicative_expression			//{ $$ = $1) + "+" + $3; }
	| additive_expression '-' multiplicative_expression			//{ $$ = $1) + "-" + $3; }
	;

multiplicative_expression
	: unary_expression											//{ $$ = $1; }
	| multiplicative_expression '*' unary_expression			//{ $$ = $1) + "*" + $3; }
	| multiplicative_expression '/' unary_expression			//{ $$ = $1) + "/" + $3; }
	| multiplicative_expression '%' unary_expression			//{ $$ = $1) + "%" + $3; }
	;

unary_expression
	: primary_expression										//{ $$ = $1; }
	| '+' unary_expression										{ $$ = $2; }
	| '-' unary_expression										{ $$ = new std::string("-" + *$2); }
	| '~' unary_expression										{ $$ = new std::string("~" + *$2); }
	| NOT unary_expression										//{ $$ = "!" + $2; }
	;

primary_expression
	: '(' expression ')'										{ $$ = new std::string("(" + *$2 + ")"); }
	| function													//{ $$ = $1; }
	| identifier												//{ $$ = $1; }
	| data_type													//{ $$ = $1; }
	;

function
	: POWER '(' expression ',' expression ')'					//{ $$ = "power(" + $3 + "," + $5 + ")"; }
	| EXP '(' expression ')'									//{ $$ = "exp(" + $3 + ")"; }
	| SQRT '(' expression ')'									//{ $$ = "sqrt(" + $3 + ")"; }
	| MATCH '(' expression ',' REGEXP ')'						//{ $$ = "match(" + $3 + "," + $5 + ")"; }
	;


data_type
	: STRING													//{ $$ = $1; }
	| INT														//{ $$ = $1; }
	| FLOAT														//{ $$ = $1; }
	| DATE														//{ $$ = $1; }
	;

%%

void yyerror(const char *s) {
	std::cout << "Parse error on line " << lineno() << " - " << s << std::endl;
	exit(-1);
}

void toLower(char *str) {
	for ( ; *str; ++str) *str = tolower(*str);        
}