%{
#include "laq.tab.h"
#include <string>
extern void toLower(char*);
%}

%option yylineno
%option c++

spacing 				[ \t\n]+
symbols					[=,&<>+/%~\(\)\|\^\-\*\.]

filter					[fF][iI][lL][tT][eE][rR]
map						[mM][aA][pP]
dot						[dD][oO][tT]
krao					[kK][rR][aA][oO]
hadamard				[hH][aA][dD][aA][mM][aA][rR][dD]

count					[cC][oO][uU][nN][tT]
sum						[sS][uU][mM]
avg						[aA][vV][gG]
min						[mM][iI][nN]
max						[mM][aA][xX]

power					[pP][oO][wW][eE][rR]
exp						[eE][xX][pP]
sqrt					[sS][qQ][rR][tT]
match					[mM][aA][tT][cC][hH]

identifier				[a-zA-Z][a-zA-Z0-9_]*
string					[\"'][^\"'\\]*(\\.[^\"'\\]*)*[\"']
int						[0-9]+
float 					([0-9]*\.[0-9]+|[0-9]+\.[0.9]*)
date					[\"'][0-9]{4}-[0-9]{2}-[0-9]{2}[\"']

or						([oO][rR]|\|\|)
and						([aA][nN][dD]|&&)
eq						(=|==)
ne						(!=|<>)
le						<=
ge						>=
regexp					{string}
left					<<
right					>>
not						([nN][oO][tT]|!)


%x MLCOMMENT REGEXPRESSION


%%


<INITIAL>{spacing}			{	; }
<INITIAL>"//".+				{	; }
<INITIAL>"/*"				{	BEGIN MLCOMMENT; }
<INITIAL>{symbols}			{	return yytext, yyleng[0]; }

<INITIAL>{filter}			{	return FILTER; }
<INITIAL>{map}				{	return MAP; }
<INITIAL>{dot}				{	return DOT; }
<INITIAL>{krao}				{	return KRAO; }
<INITIAL>{hadamard}			{	return HADAMARD; }

<INITIAL>{count}			{	return COUNT; }
<INITIAL>{sum}				{	return SUM; }
<INITIAL>{avg}				{	return AVG; }
<INITIAL>{min}				{	return MIN; }
<INITIAL>{max}				{	return MAX; }

<INITIAL>{power}			{	return POWER; }
<INITIAL>{exp}				{	return EXP; }
<INITIAL>{sqrt}				{	return SQRT; }

<INITIAL>{int}				{	yylval->str = new std::string( yytext, yyleng );
								return INT;
							}
<INITIAL>{float}			{	yylval->str = new std::string( yytext, yyleng );
								return FLOAT;
							}
<INITIAL>{date}				{	yylval->str = new std::string( yytext, yyleng );
								return DATE; 
							}

<INITIAL>{or}				{	return OR; }
<INITIAL>{and}				{	return AND; }
<INITIAL>{eq}				{	return EQ; }
<INITIAL>{ne}				{	return NE; }
<INITIAL>{le}				{	return LE; }
<INITIAL>{ge}				{	return GE; }
<INITIAL>{match}			{	BEGIN REGEXPRESSION;
								return MATCH;
							}
<INITIAL>{left}				{	return LEFT; }
<INITIAL>{right}			{	return RIGHT; }
<INITIAL>{not}				{	return NOT; }

<INITIAL>{string}			{	yylval->str = new std::string( yytext, yyleng );
								return STRING;
							}
<INITIAL>{identifier}		{	toLower(yytext, yyleng);
								yylval->str = new std::string( yytext, yyleng );
								return IDENTIFIER;
							}

<MLCOMMENT>"*/"				{	BEGIN INITIAL; }
<MLCOMMENT>(.|\n)			{	; }

<REGEXPRESSION>{spacing}	{	; }
<REGEXPRESSION>{symbols}	{	return yytext, yyleng[0]; }
<REGEXPRESSION>{string}		{	yylval->str = new std::string( yytext, yyleng );
								BEGIN INITIAL;
								return REGEXP;
							}

%%