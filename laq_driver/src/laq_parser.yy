/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "2.6.90.8-d4fe"
%defines
%define parser_class_name { laq_parser }

%code requires {
# include <string>
namespace laq { class driver; }
}

// The parsing context.
%parse-param  { laq::driver* driver }
%lex-param    { laq::driver* driver }

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver->file;
};

%debug
%error-verbose

// Symbols.
%union {
  std::string *sval;
}

%code {
# include "include/laq_driver.hpp"
}

%token END 0 "end of file"
%token FILTER LIFT DOT KRAO HADAMARD
%token COUNT SUM AVG MIN MAX
%token POWER EXP SQRT MATCH
%token OR AND EQ NE LE GE LEFT RIGHT NOT
%token RETURN

%token <sval> IDENTIFIER INT FLOAT STRING DATE REGEXP

%type <sval> ident
%type <sval> product
%type <sval> bang
%type <sval> expression
%type <sval> logical_and_expression
%type <sval> inclusive_or_expression
%type <sval> exclusive_or_expression
%type <sval> and_expression
%type <sval> equality_expression
%type <sval> relational_expression
%type <sval> shift_expression
%type <sval> additive_expression
%type <sval> multiplicative_expression
%type <sval> unary_expression
%type <sval> primary_expression
%type <sval> function
%type <sval> literal

%printer    { yyoutput  <<  *$$; } <sval>
%destructor { delete $$; } <sval>

%%
%start laquery;

laquery
  : "end of file"                                         {  /*EMPTY*/ }
  | statement                                             {  }
  | laquery statement                                     {  }
  ;

statement
  : IDENTIFIER '=' product '(' ident ',' ident ')'        { if(driver->var_exists(*$1))
                                                              driver->error("Error: Redeclared variable " + *$1);
                                                            driver->add_var(*$1);
                                                            driver->insertStatement(*$1, *$3, std::vector<std::string> {*$5, *$7});
                                                            delete $1;
                                                            delete $3;
                                                            delete $5;
                                                            delete $7;
                                                          }
  | IDENTIFIER '=' bang '(' ident ')'                     { if(driver->var_exists(*$1))
                                                              driver->error("Error: Redeclared variable " + *$1);
                                                            driver->add_var(*$1);
                                                            driver->insertStatement(*$1, *$3, std::vector<std::string> {*$5});
                                                            delete $1;
                                                            delete $3;
                                                            delete $5;
                                                          }
  | IDENTIFIER '=' FILTER '(' expression ')'              { if(driver->var_exists(*$1))
                                                              driver->error("Error: Redeclared variable " + *$1);
                                                            driver->add_var(*$1);
                                                            std::vector<std::string> expvars = driver->clear_exp_vars();
                                                            driver->insertStatement(*$1, "filter", expvars, *$5);
                                                            delete $1;
                                                            delete $5;
                                                          }
  | IDENTIFIER '=' LIFT '(' expression ')'                 { if(driver->var_exists(*$1))
                                                              driver->error("Error: Redeclared variable " + *$1);
                                                            driver->add_var(*$1);
                                                            std::vector<std::string> expvars = driver->clear_exp_vars();
                                                            driver->insertStatement(*$1, "lift", expvars, *$5);
                                                            delete $1;
                                                            delete $5;
                                                          }
  | RETURN '(' varlist ')'                                { std::vector<std::string> varlist = driver->clear_exp_vars();
                                                            driver->insertStatement("$$", "return", varlist);
                                                          }
  ;

varlist
  : ident                                                 { driver->add_exp_var(*$1);
                                                            delete $1;
                                                          }
  | varlist ',' ident                                     { driver->add_exp_var(*$3);
                                                            delete $3;
                                                          }
  ;

ident
  : IDENTIFIER                                            { if(!driver->var_exists(*$1))
                                                              driver->error("Error: Undeclared variable " + *$1);
                                                            $$ = $1;
                                                          }
  | IDENTIFIER '.' IDENTIFIER                             { $$ = new std::string(*$1 + "_" + *$3);
                                                            driver->addDatabaseVar(*$1, *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

product
  : DOT                                                   { $$ = new std::string("dot"); }
  | KRAO                                                  { $$ = new std::string("krao"); }
  | HADAMARD                                              { $$ = new std::string("hadamard"); }
  ;

bang
  : COUNT                                                 { $$ = new std::string("count"); }
  | SUM                                                   { $$ = new std::string("sum"); }
  | AVG                                                   { $$ = new std::string("avg"); }
  | MIN                                                   { $$ = new std::string("min"); }
  | MAX                                                   { $$ = new std::string("max"); }
  ;

expression
  : logical_and_expression                                { $$ = $1; }
  | expression OR logical_and_expression                  { $$ = new std::string(*$1 + "||" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

logical_and_expression
  : inclusive_or_expression                               { $$ = $1; }
  | logical_and_expression AND inclusive_or_expression    { $$ = new std::string(*$1 + "&&" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

inclusive_or_expression
  : exclusive_or_expression                               { $$ = $1; }
  | inclusive_or_expression '|' exclusive_or_expression   { $$ = new std::string(*$1 + "|" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

exclusive_or_expression
  : and_expression                                        { $$ = $1; }
  | exclusive_or_expression '^' and_expression            { $$ = new std::string(*$1 + "^" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

and_expression
  : equality_expression                                   { $$ = $1; }
  | and_expression '&' equality_expression                { $$ = new std::string(*$1 + "&" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

equality_expression
  : relational_expression                                 { $$ = $1; }
  | equality_expression EQ relational_expression          { $$ = new std::string(*$1 + "==" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | equality_expression NE relational_expression          { $$ = new std::string(*$1 + "!=" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

relational_expression
  : shift_expression                                      { $$ = $1; }
  | relational_expression '<' shift_expression            { $$ = new std::string(*$1 + "<" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | relational_expression '>' shift_expression            { $$ = new std::string(*$1 + ">" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | relational_expression LE shift_expression             { $$ = new std::string(*$1 + "<=" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | relational_expression GE shift_expression             { $$ = new std::string(*$1 + ">=" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

shift_expression
  : additive_expression                                   { $$ = $1; }
  | shift_expression LEFT additive_expression             { $$ = new std::string(*$1 + " << " + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | shift_expression RIGHT additive_expression            { $$ = new std::string(*$1 + ">>" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

additive_expression
  : multiplicative_expression                             { $$ = $1; }
  | additive_expression '+' multiplicative_expression     { $$ = new std::string(*$1 + "+" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | additive_expression '-' multiplicative_expression     { $$ = new std::string(*$1 + "-" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

multiplicative_expression
  : unary_expression                                      { $$ = $1; }
  | multiplicative_expression '*' unary_expression        { $$ = new std::string(*$1 + "*" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | multiplicative_expression '/' unary_expression        { $$ = new std::string(*$1 + "/" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  | multiplicative_expression '%' unary_expression        { $$ = new std::string(*$1 + "%" + *$3);
                                                            delete $1;
                                                            delete $3;
                                                          }
  ;

unary_expression
  : primary_expression                                    { $$ = $1; }
  | '+' unary_expression                                  { $$ = $2; }
  | '-' unary_expression                                  { $$ = new std::string("-" + *$2);
                                                            delete $2;
                                                          }
  | '~' unary_expression                                  { $$ = new std::string("~" + *$2);
                                                            delete $2;
                                                          }
  | NOT unary_expression                                  { $$ = new std::string("!" + *$2);
                                                            delete $2;
                                                          }
  ;

primary_expression
  : '(' expression ')'                                    { $$ = new std::string("(" + *$2 + ")");
                                                            delete $2;
                                                          }
  | function                                              { $$ = $1; }
  | ident                                                 { driver->add_exp_var(*$1);
                                                            $$ = new std::string("args[" +
                                                                    std::to_string(driver->count_exp_vars() - 1) +
                                                                    "]");
                                                            delete $1;
                                                          }
  | literal                                               { $$ = $1; }
  ;

function
  : POWER '(' expression ',' expression ')'               { $$ = new std::string("power(" + *$3 + "," + *$5 + ")");
                                                            delete $3;
                                                            delete $5;
                                                          }
  | EXP '(' expression ')'                                { $$ = new std::string("exp(" + *$3 + ")");
                                                            delete $3;
                                                          }
  | SQRT '(' expression ')'                               { $$ = new std::string("sqrt(" + *$3 + ")");
                                                            delete $3;
                                                          }
  | MATCH '(' expression ',' REGEXP ')'                   { $$ = new std::string("match(" + *$3 + "," + *$5 + ")");
                                                            delete $3;
                                                            delete $5;
                                                          }
  ;

literal
  : STRING                                                { $$ = $1; }
  | INT                                                   { $$ = $1; }
  | FLOAT                                                 { $$ = $1; }
  | DATE                                                  { $$ = $1; }
  ;

%%

void
yy::laq_parser::error(const yy::laq_parser::location_type& l,
         const std::string& m) {
  driver->error(l, m);
}
