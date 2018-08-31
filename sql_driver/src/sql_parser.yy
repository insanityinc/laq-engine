/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "2.6.90.8-d4fe"
%defines
%define parser_class_name { sql_parser }

%code requires {
# include <string>
namespace sql { class driver; }
}

// The parsing context.
%parse-param  { sql::driver* driver }
%lex-param    { sql::driver* driver }

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
  int integer;
}

%code {
# include "include/sql_driver.hpp"
}

%token END 0 "end of file"
%token SELECT WHERE GROUPBY ORDERBY HAVING AS
%token AND OR EXISTS BETWEEN JOIN INNER LEFT RIGHT FULL ON IN ANDOP BEFORE IS
%token LIKE REGEX VIRGULA PVIRGULA

%token <sval> NAME
%token <sval> ATTRIBUTE
%token <sval> BBOP
%token <sval> BOP
%token <sval> IBOP
%token <sval> NOT
%token <sval> ASC
%token <sval> DESC
%token <sval> DATE
%token <sval> CONSTANT
%token <sval> BOOL
%token <sval> ANY
%token <sval> ALL
%token <sval> FROM


%type <integer> Literal
%type <sval> Join
%type <integer> ExpR
%type <integer> Exp
%type <integer> Term
%type <integer> Factor
%type <sval> groupbyList
%type <sval> groupbyListSub
%type <sval> order
%type <integer> Args1
%type <integer> Args

%left AND
%left OR
%left IBOP
%left BBOP

%printer    { yyoutput  <<  *$$; } <sval>
%destructor { delete $$; } <sval>

%%
%start Final;

Final
  : SelectBlock
  ;

SelectBlock
  : SELECT selectList
    FROM fromList
    WHERE_
    GROUPBY_
    ORDERBY_
    PVIRGULA                                  { driver->l = & driver->trees[0];
                                                driver->mainGraph->newRoot(
                                                  sql::giveMeRoot(driver->mainGraph->root));
                                                sql::print_tree();
                                                driver->l->rewrite(0);
                                                sql::print_tree();
                                                sql::resolve(0);
                                                sql::returnf();
                                              }
  ;

WHERE_
  : WHERE whereList                           {  }
  |                                           {  }
  ;

GROUPBY_
  : GROUPBY groupbyList                       {  }
  |                                           {  }
  ;


ORDERBY_
  : ORDERBY orderbyList                       {  }
  |                                           {  }
  ;

selectList
  : selectListN                               { driver->mainGraph->add_table("lineitem", "shipdate", "dimension");
                                                driver->mainGraph->add_table("lineitem", "extendedprice", "measure");
                                                driver->mainGraph->add_table("lineitem", "discount", "measure");
                                                driver->mainGraph->add_table("lineitem", "quantity", "measure");
                                              }
  | IBOP                                      { driver->mainGraph->add_select("*",""); }
  ;

selectListN
  : selectListNSub                            {  }
  | selectListN VIRGULA  selectListNSub       {  }
  ;

selectListNSub
  : Term                                      { driver->mainGraph->add_select(driver->types[$1].expr,"");
                                                /*std::cout << driver->types[$1].expr << "\n"*/
                                              }
  | Term AS NAME                              { driver->mainGraph->add_select(driver->types[$1].expr,*$3); }
  ;

fromList
  : subfromList                               {  }
  | fromList VIRGULA subfromList              {  }
  ;

subfromList
  : NAME                                      { driver->mainGraph->newRoot(*$1); }
  | Join NAME                                 {  }
  | Join NAME ON Literal '=' Literal          { driver->mainGraph->add_join(
                                                  driver->types[$6].type,
                                                  sql::getTable(driver->types[$4].type),
                                                  sql::getTable(driver->types[$6].type));
                                              }
  | Join NAME AS NAME                         { driver->mainGraph->tables[*$4] = driver->mainGraph->tables[*$2]; }
  | Join NAME AS NAME ON Literal '=' Literal  { driver->mainGraph->add_join(
                                                  driver->types[$8].type,
                                                  sql::getTable(driver->types[$6].type),
                                                  sql::getTable(driver->types[$8].type));
                                                driver->mainGraph->tables[*$4] = driver->mainGraph->tables[*$2]; }
  | '{' SelectBlock '}'                       {  }
  | '{' SelectBlock '}' AS NAME               {  }
  ;

Join
  : JOIN                                      { /*$$ = "Normal"*/ }
  | INNER JOIN                                { /*$$ = "Inner"*/ }
  | LEFT JOIN                                 { /*$$ = "Left"*/ }
  | RIGHT JOIN                                { /*$$ = "Rigth"*/ }
  | FULL JOIN                                 { /*$$ = "Full"*/ }
  ;

whereList
  : Exp                                       { std::cout << "where" << std::endl; }
  ;

ExpR
  : Exp AND Exp                               { $$ = $1;
                                                std::cout << "3" << std::endl;
                                                change_trees(
                                                  join_trees(driver->trees[$1], driver->trees[$3], "AND"),
                                                  $1);
                                              }
  | Exp OR  Exp                               { $$ = $1;
                                                change_trees(
                                                  join_trees(driver->trees[$1], driver->trees[$3], "OR"),
                                                  $1);
                                              }
  ;

Exp
  : Term                                      { $$ = driver->itr;
                                                driver->trees.push_back(
                                                  sql::create_tree(driver->types[$1].expr, driver->types[$1].type));
                                                driver->itr++;
                                              }
  | ExpR                                      { $$ = $1; }
  | '(' ExpR ')'                              { $$ = $2; }
  | Literal BETWEEN Term AND Term             { $$ = driver->itr;
                                                std::string s = driver->types[$1].expr;
                                                std::string s2 = driver->types[$1].expr;
                                                s.append("<=");
                                                s2.append(">=");
                                                s.append(driver->types[$3].expr);
                                                s2.append(driver->types[$5].expr);
                                                driver->trees.push_back(
                                                  sql::create_tree(s,driver->types[$1].type));
                                                driver->itr++;
                                                driver->trees.push_back(
                                                  sql::create_tree(s2,driver->types[$1].type));
                                                driver->itr++;
                                                change_trees(
                                                  join_trees(driver->trees[$1], driver->trees[$3], "AND"),
                                                  $1);
                                              }
  ;

Term
  : Factor                                      { $$ = $1; }
  | '(' Term ')'                                { $$ = $2; }
  | Term BBOP Term                              { $$ = $1;
                                                  std::cout << "1" << std::endl;
                                                  driver->types[$1].expr.append(*$2);
                                                  driver->types[$1].expr.append(driver->types[$3].expr);
                                                  if (driver->types[$1].type.size() && driver->types[$3].type.size()) {
                                                    driver->mainGraph->add_join(
                                                      driver->types[$3].type,
                                                      sql::getTable(driver->types[$1].type),
                                                      sql::getTable(driver->types[$3].type));
                                                    driver->types[$1].type = "JOIN";
                                                  } else {
                                                    driver->mainGraph->add_map_filter(
                                                      driver->types[$1].type,
                                                      driver->types[$1].expr);
                                                    driver->types[$1].type = "NOT JOIN";
                                                  }
                                                  std::cout << "2\n";
                                                }

  | Term IBOP Term                              { $$ = $1;
                                                  driver->types[$1].expr.append(*$2);
                                                  driver->types[$1].expr.append(driver->types[$3].expr);
                                                  driver->types[$1].type.append(driver->types[$3].type);
                                                }
  ;

Factor
  : Literal                                     { $$ = $1; }
  | NAME '(' Args ')'                           { $$ = $3;
                                                  std::string s = "";
                                                  s.append(*$1);
                                                  s.append("(");
                                                  s.append(driver->types[$3].expr);
                                                  s.append(")");
                                                  driver->types[$3].expr = s;
                                                }
  | NOT Factor                                  { $$ = $2;
                                                  std::string s = "";
                                                  s.append(*$1);
                                                  s.append(" ");
                                                  s.append(driver->types[$2].expr);
                                                  driver->types[$2].expr = s;
                                                }
  ;

Args
  : Args1                                       { $$ = $1; }
  |                                             {  }
  ;

Args1
  : Term                                        { $$ = $1; }
  | Args1 VIRGULA  Term                         { $$ = $1;
                                                  driver->types[$1].expr.append(",");
                                                  driver->types[$1].expr.append(driver->types[$3].expr);
                                                  driver->types[$1].type.append(driver->types[$3].type);
                                                }
  | Args1 ' ' Term                              { $$ = $1;
                                                  driver->types[$1].expr.append(" ");
                                                  driver->types[$1].expr.append(driver->types[$3].expr);
                                                  driver->types[$1].type.append(driver->types[$3].type);
                                                }
  ;
  /*
  | NAME IN Inlist                              {  }
  | NAME BETWEEN Literal AND Literal            {  }
  | EXISTS '(' SelectBlock ')' ';'              {  }
  | Literal LIKE REGEX                          {  } // regular expression
  ;
  */

groupbyList
  : groupbyListSub                              { std::cout << "gb" << std::endl; }
  | groupbyList VIRGULA groupbyListSub          {  }
  ;

groupbyListSub
  : HAVING NAME '(' Literal ')' BBOP Literal    {  }
  | Literal                                     { std::string Table = sql::getTable(driver->types[$1].expr);
                                                  std::string s = Table;
                                                  s.append(".");
                                                  s.append(driver->types[$1].expr);
                                                  driver->mainGraph->add_groupby(Table,s);
                                                }
  ;

orderbyList
  : orderbyListSub                              {  }
  | orderbyList VIRGULA orderbyListSub          {  }
  ;

orderbyListSub
  : Literal                                     {  }
  | Literal order                               {  }
  ;

order
  : ASC                                         {  }
  | DESC                                        {  }
  ;

/*
OL
  : AND                                         { $$ = $1; }
  | ANDOP                                       { $$ = $1; }
  | OR                                          { $$ = $1; }
  ;
*/


Literal
  : NAME                                        { std::string s = sql::getTable(*$1);
                                                  s.append(".");
                                                  $$ = driver->itr2;
                                                  if (!s.compare(".")) {
                                                    sql::driver::Par novo;
                                                    novo.type = "";
                                                    novo.expr = *$1;
                                                    driver->types.push_back(novo);
                                                    driver->itr2 ++;
                                                  } else {
                                                    s.append(*$1);
                                                    sql::driver::Par novo;
                                                    novo.type = s;
                                                    novo.expr = s;
                                                    driver->types.push_back(novo);
                                                    driver->itr2 ++;
                                                  }
                                                }
  | ATTRIBUTE                                   { $$ = driver->itr2;
                                                  std::cout << *$1 << std::endl;
                                                  std::string s = sql::getTable(*$1);
                                                  sql::driver::Par novo;
                                                  novo.type = s;
                                                  novo.expr = *$1;
                                                  driver->types.push_back(novo);
                                                  driver->itr2 ++;
                                                }
  | DATE                                        { $$ = driver->itr2;
                                                  sql::driver::Par novo;
                                                  novo.type = "";
                                                  novo.expr = *$1;
                                                  driver->types.push_back(novo);
                                                  driver->itr2 ++;
                                                }
  | CONSTANT                                    { $$ = driver->itr2;
                                                  sql::driver::Par novo;
                                                  novo.type = "";
                                                  novo.expr = *$1;
                                                  driver->types.push_back(novo);
                                                  driver->itr2 ++;
                                                }
  | BOOL                                        { $$ = driver->itr2;
                                                  sql::driver::Par novo;
                                                  novo.type = "";
                                                  novo.expr = *$1;
                                                  driver->types.push_back(novo);
                                                  driver->itr2 ++;
                                                }
  | ANY                                         { $$ = driver->itr2;
                                                  sql::driver::Par novo;
                                                  novo.type = "";
                                                  novo.expr = *$1;
                                                  driver->types.push_back(novo);
                                                  driver->itr2 ++;
                                                }
  | ALL                                         { $$ = driver->itr2;
                                                  sql::driver::Par novo;
                                                  novo.type = "";
                                                  novo.expr = *$1;
                                                  driver->types.push_back(novo);
                                                  driver->itr2 ++;
                                                }
  ;

/*
Expr
  : Literal                                     { $$ = $1; }
  | NOT Expr                                    { $$ = $1 + $2; }
  | Expr BOP Expr                               { $$ = $1+$2+$3; }
  | Expr '=' Expr                               { $$ = $1+$2+$3; }
  | NAME '(' Expr ')'                           { $$ = $1+$2+$3+$4; }
  | '('SelectBlock')'                           {  }
  ;
*/

/*
Inlist
  : Literal                                     { $$ = $1; }
  | Inlist ',' Literal                          { $$ = $3; }
  ;
*/

%%

void
yy::sql_parser::error(const yy::sql_parser::location_type& l,
                      const std::string& m) {
  driver->error(l, m);
}
