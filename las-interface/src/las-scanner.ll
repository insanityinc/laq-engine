/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
%{ /* -*- C++ -*- */
# include <cstdlib>
# include <cerrno>
# include <climits>
# include <string>
# include "las-driver.hh"
# include "las-parser.hh"

/* Work around an incompatibility in flex (at least versions
   2.5.31 through 2.5.33): it generates code that does
   not conform to C89.  See Debian bug 333231
   <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.   */
# undef yywrap
# define yywrap() 1

/* By default yylex returns int, we use token_type.
   Unfortunately yyterminate by default returns 0, which is
   not of token_type.   */
#define yyterminate() return token::END
%}

%option noyywrap nounput batch debug

blank                   [ \t\r]+
symbols                 [=,&<>+/%~\(\)\|\^\-\*\.]

filter                  [fF][iI][lL][tT][eE][rR]
map                     [mM][aA][pP]
dot                     [dD][oO][tT]
krao                    [kK][rR][aA][oO]
hadamard                [hH][aA][dD][aA][mM][aA][rR][dD]

count                   [cC][oO][uU][nN][tT]
sum                     [sS][uU][mM]
avg                     [aA][vV][gG]
min                     [mM][iI][nN]
max                     [mM][aA][xX]

power                   [pP][oO][wW][eE][rR]
exp                     [eE][xX][pP]
sqrt                    [sS][qQ][rR][tT]
match                   [mM][aA][tT][cC][hH]

identifier              [a-zA-Z][a-zA-Z0-9_]*
string                  [\"'][^\"'\\]*(\\.[^\"'\\]*)*[\"']
int                     [0-9]+
float                   ([0-9]*\.[0-9]+|[0-9]+\.[0.9]*)
date                    [\"'][0-9]{4}-[0-9]{2}-[0-9]{2}[\"']

or                      ([oO][rR]|\|\|)
and                     ([aA][nN][dD]|&&)
eq                      (=|==)
ne                      (!=|<>)
le                      <=
ge                      >=
left                    <<
right                   >>
not                     ([nN][oO][tT]|!)


%{
# define YY_USER_ACTION yylloc->columns (yyleng);
%}

%x MLCOMMENT REGEXPRESSION

%%
%{
  yylloc->step ();
  typedef yy::las_parser::token token;
  void toLower(std::string& str);
%}

<INITIAL>{blank}            { yylloc->step(); }
<INITIAL>[\n]+              { yylloc->lines( yyleng );
                              yylloc->step();
                            }
<INITIAL>"//".+             { }
<INITIAL>"/*"               { BEGIN MLCOMMENT; }
<INITIAL>{symbols}          { return yy::las_parser::token_type( yytext[0] ); }

<INITIAL>{filter}           { return token::FILTER; }
<INITIAL>{map}              { return token::MAP; }
<INITIAL>{dot}              { return token::DOT; }
<INITIAL>{krao}             { return token::KRAO; }
<INITIAL>{hadamard}         { return token::HADAMARD; }

<INITIAL>{count}            { return token::COUNT; }
<INITIAL>{sum}              { return token::SUM; }
<INITIAL>{avg}              { return token::AVG; }
<INITIAL>{min}              { return token::MIN; }
<INITIAL>{max}              { return token::MAX; }

<INITIAL>{power}            { return token::POWER; }
<INITIAL>{exp}              { return token::EXP; }
<INITIAL>{sqrt}             { return token::SQRT; }

<INITIAL>{int}              { yylval->sval = new std::string( yytext );
                              return token::INT;
                            }
<INITIAL>{float}            { yylval->sval = new std::string( yytext );
                              return token::FLOAT;
                            }
<INITIAL>{date}             { yylval->sval = new std::string( yytext );
                              return token::DATE; 
                            }

<INITIAL>{or}               { return token::OR; }
<INITIAL>{and}              { return token::AND; }
<INITIAL>{eq}               { return token::EQ; }
<INITIAL>{ne}               { return token::NE; }
<INITIAL>{le}               { return token::LE; }
<INITIAL>{ge}               { return token::GE; }
<INITIAL>{match}            { BEGIN REGEXPRESSION;
                              return token::MATCH;
                            }
<INITIAL>{left}             { return token::LEFT; }
<INITIAL>{right}            { return token::RIGHT; }
<INITIAL>{not}              { return token::NOT; }

<INITIAL>{string}           { yylval->sval = new std::string( yytext );
                              return token::STRING;
                            }
<INITIAL>{identifier}       { yylval->sval = new std::string( yytext );
                              toLower( *(yylval->sval) );
                              return token::IDENTIFIER;
                            }
<INITIAL>.                  { driver.error (*yylloc, "invalid character: " + std::string( yytext ) ); }

<MLCOMMENT>"*/"             { BEGIN INITIAL; }
<MLCOMMENT>.                { }
<MLCOMMENT>[\n]+            { yylloc->lines( yyleng );
                              yylloc->step();
                            }

<REGEXPRESSION>{blank}      { yylloc->step(); }
<REGEXPRESSION>[\n]+        { yylloc->lines( yyleng );
                              yylloc->step();
                            }
<REGEXPRESSION>{identifier} { yylval->sval = new std::string( yytext );
                              toLower( *(yylval->sval) );
                              return token::IDENTIFIER;
                            }
<REGEXPRESSION>{symbols}    { return yy::las_parser::token_type( yytext[0] ); }
<REGEXPRESSION>{string}     { yylval->sval = new std::string( yytext );
                              BEGIN INITIAL;
                              return token::REGEXP;
                            }
<REGEXPRESSION>.            { driver.error (*yylloc, "invalid character on regular expression: " + std::string( yytext ) ); }

%%


void
las::driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
  {
    error ("cannot open " + file + ": " + strerror(errno));
    exit (EXIT_FAILURE);
  }
}

void
las::driver::scan_end ()
{
  fclose (yyin);
}

void
toLower(std::string& str)
{
  for (char& c : str)
    c = (char) tolower(c);
}

