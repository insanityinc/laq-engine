/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
%{ /* -*- C++ -*- */
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <string>
#include "include/sql_driver.hpp"

/* Work around an incompatibility in flex (at least versions
   2.5.31 through 2.5.33): it generates code that does
   not conform to C89.  See Debian bug 333231
   <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.   */
#undef yywrap
#define yywrap() 1

/* By default yylex returns int, we use token_type.
   Unfortunately yyterminate by default returns 0, which is
   not of token_type.   */
#define yyterminate() return token::END
%}

%option noyywrap nounput batch debug

blank           [ \t\r]+

as              [Aa][Ss]
before          [Bb][Ee][Ff][Oo][Rr][Ee]
join            [Jj][Oo][Ii][Nn]
right           [Rr][Ii][Gg][Hh][Tt]
left            [Ll][Ee][Ff][Tt]
full            [Ff][Uu][Ll][Ll]
inner           [Ii][Nn][Nn][Ee][Rr]
on              [Oo][Nn]
between         [Bb][Ee][Tt][Ww][Ee][Ee][Nn]
exists          [Ee][Xx][Ii][Ss][Tt][Ss]
from            [Ff][Rr][Oo][Mm]
groupby         [Gg][Rr][Oo][Uu][Pp][ ]*[Bb][Yy]
orderby         [Oo][Rr][Dd][Ee][Rr][ ]*[Bb][Yy]
having          [Hh][Aa][Vv][Ii][Nn][Gg]
in              [Ii][Nn]
is              [Ii][Ss]
not             [Nn][Oo][Tt]
select          [Ss][Ee][Ll][Ee][Cc][Tt]
where           [Ww][Hh][Ee][Rr][Ee]
asc             [Aa][Ss][Cc]
desc            [Dd][Ee][Ss][Cc]
any             [Aa][Nn][Yy]
all             [Aa][Ll][Ll]
substring       [Ss][Uu][Bb][Ss][Tt][Rr]([In][Nn][Gg])?

Or              [oO][rR]
and             [aA][nN][dD]
eq              (=|==)
ne              (!=|<>)
le              "<="
l               "<"
g               ">"
ge              ">="
mais            "+"
div             "/"
mul             "*"
men             "-"
virg            ","
pvirg           ";"

Not             ([nN][oO][tT]|!)
true            [Tt][Rr][Uu][Ee]
false           [Ff][Aa][Ll][Ss][Ee]

int             [0-9]+
float           ([0-9]*\.[0-9]+|[0-9]+\.[0.9]*)
attribute       [A-Za-z][A-Za-z0-9_]*[.][A-Za-z][A-Za-z0-9_]*
name            [A-Za-z':][:'A-Za-z0-9_]*
Date            [dD][aA][tT][eE]{blank}['][:0-9]*[']


%{
#define YY_USER_ACTION yylloc->columns (yyleng);
%}

%%

%{
  yylloc->step ();
  typedef yy::sql_parser::token token;
  void toLower(std::string& str);
%}

<INITIAL>{blank}               { yylloc->step(); }
<INITIAL>[\n]+                 { yylloc->lines( yyleng );
                                 yylloc->step();
                               }

<INITIAL>{virg}                { return token::VIRGULA; }
<INITIAL>{pvirg}               { return token::PVIRGULA; }
<INITIAL>{as}                  { return token::AS; }
<INITIAL>{before}              { return token::BEFORE; }
<INITIAL>{full}                { return token::FULL; }
<INITIAL>{inner}               { return token::INNER; }
<INITIAL>{on}                  { return token::ON; }
<INITIAL>{between}             { return token::BETWEEN; }
<INITIAL>{exists}              { return token::EXISTS; }
<INITIAL>{from}                { return token::FROM; }
<INITIAL>{groupby}             { return token::GROUPBY; }
<INITIAL>{orderby}             { return token::ORDERBY; }
<INITIAL>{having}              { return token::HAVING; }
<INITIAL>{in}                  { return token::IN; }
<INITIAL>{is}                  { return token::IS; }
<INITIAL>{Not}                 { return token::NOT; }
<INITIAL>{select}              { return token::SELECT; }
<INITIAL>{where}               { return token::WHERE; }
<INITIAL>{asc}                 { return token::ASC; }
<INITIAL>{desc}                { return token::DESC; }
<INITIAL>{any}                 { return token::ANY; }
<INITIAL>{all}                 { return token::ALL; }
<INITIAL>{right}               { return token::RIGHT; }


<INITIAL>{int}                 { yylval->sval = new std::string( yytext );
                                 return token::CONSTANT;
                               }
<INITIAL>{float}               { yylval->sval = new std::string( yytext );
                                 return token::CONSTANT;
                               }
<INITIAL>{Date}                { yylval->sval = new std::string( yytext );
                                 return token::DATE;
                               }
<INITIAL>{true}                { yylval->sval = new std::string( yytext );
                                 return token::BOOL;
                               }
<INITIAL>{false}               { yylval->sval = new std::string( yytext );
                                 return token::BOOL;
                               }



<INITIAL>{Or}                  { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{and}                 { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{le}                  { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{l}                   { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{g}                   { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{ge}                  { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{eq}                  { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{ne}                  { yylval->sval = new std::string( yytext );
                                 return token::BBOP;
                               }
<INITIAL>{mul}                 { yylval->sval = new std::string( yytext );
                                 return token::IBOP;
                               }
<INITIAL>{mais}                { yylval->sval = new std::string( yytext );
                                    return token::IBOP;
                               }
<INITIAL>{div}                 { yylval->sval = new std::string( yytext );
                                 return token::IBOP;
                               }
<INITIAL>{men}                 { yylval->sval = new std::string( yytext );
                                 return token::IBOP;
                               }
<INITIAL>{left}                { yylval->sval = new std::string( yytext );
                                 return token::IBOP;
                               }
<INITIAL>{attribute}           { yylval->sval = new std::string( yytext );
                                 return token::ATTRIBUTE;
                               }
<INITIAL>{name}                { yylval->sval = new std::string( yytext );
                                 return token::NAME;
                               }

%%


void
sql::driver::scan_begin() {
  yy_flex_debug = trace_scanning;
  if (file.empty() || file == "-") {
    yyin = stdin;
  }
  else if (!(yyin = fopen(file.c_str(), "r"))) {
    error("Cannot open " + file + ": " + strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void
sql::driver::scan_end() {
  fclose(yyin);
}
