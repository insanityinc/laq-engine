/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
#ifndef SQL_DRIVER_INCLUDE_SQL_DRIVER_H_
#define SQL_DRIVER_INCLUDE_SQL_DRIVER_H_
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "build/sql_parser.hpp"

// Tell Flex the lexer's prototype ...
#define YY_DECL                                 \
  yy::sql_parser::token_type                    \
  yylex(yy::sql_parser::semantic_type* yylval,  \
        yy::sql_parser::location_type* yylloc,  \
        sql::driver* driver)
// ... and declare it for the parser's sake
YY_DECL;

namespace sql {

// Conducting the whole scanning and parsing of SQL
class driver {
 public:
  driver();
  virtual ~driver();

  // Handling the scanner
  void scan_begin();
  void scan_end();
  bool trace_scanning;

  // Run the parser. Return 0 on success
  int parse(const std::string& f);
  std::string file;
  bool trace_parsing;

  // Error handling
  void error(const yy::location& l, const std::string& m);
  void error(const std::string& m);

  class Ltree;
  class Graph;
  typedef struct {
    std::string expr;
    std::string type;
  } Par;

  // Variables
  std::string a;
  Graph *mainGraph;
  Graph *g;
  Ltree *l;
  std::vector<Ltree> trees;
  int itr;
  std::vector<Par> types;
  int itr2;

  bool has(std::vector<std::string> v, std::string s);

  std::string giveMeRoot(std::string root);

  driver::Ltree create_tree(std::string term, std::string type);

  driver::Ltree join_trees(driver::Ltree l1, driver::Ltree l2, std::string type);

  std::string getTable(std::string attribute);

  std::vector<int> subvector (std::vector<int> v1, std::vector<int> v2);

  std::string relacao_entre(std::vector<std::string> v, std::string s);

  std::string relacao_entre_arrays(std::vector<std::string> v, std::vector<std::string> s);

  void next();

  void joinAtrD(std::string s, std::vector<std::string> v);

  void joinAtr(std::string start);

  void joinFiltersD(std::vector<std::string> v);

  void joinFilters(std::string start);

  void joinGroupbyAux(std::string gb);

  void joinGroupby(std::string start);

  void removeTable(std::string start);

  std::string giveMeStart(std::string root);

  std::vector<std::string> driver::resolveS_aux(std::vector<std::string> v);

  void graphWorkAux(int x);

  void graphWork(std::string type);

  void merge(std::vector<std::string> v);

  void dot_all(int ind);

  void dot_all();

  void driver::print_tables2();

  void driver::dot_all_aux(std::string start);

  void dot_if_needed();

  void resolveS(int indice, std::string type);

  void resolve(int indice);

  void returnf();

  void change_trees(driver::Ltree t2, int ind);

  void copy_tree(driver::Ltree t);

  void joinFiltersM(std::vector<std::string> v);

  void print_tree();

  void print_trees();

  void print_tables();

  void print_joins();

  void final();

  void select_asterisco();

  void select_term(int s1);

  void select_term_name(int s1,const std::string& dolar3);

  void subfromlist_name(const std::string& dolar1);

  void subfromlist_join_literal_literal(int  dolar6, int dolar4);

  void subfromlist_join_nameAS(const std::string& dolar4, const std::string& dolar2);

  void subfromlist_join_as_literal_literal(const std::string& dolar2,const std::string& dolar4,int dolar6, int dolar8);

  void expr_and(int s1, int  s3);

  void expr_or(int s1, int s3);

  void exp_term(int s1);

  void term_bbop_term(int s1, const std::string& dolar2, int s3);

  void term_ibop_term(int s1, const std::string& dolar2, int s3);

  void factor_name_arg(const std::string& dolar1,int s3);

  void factor_not_factor(const std::string& dolar1, int s2);

  void args1_virgula(int s1, int s3);

  void args1_espaco(int  dolar1, int  dolar3);

  void groupbyListSub_literal(int dolar1);

  void literal_name(const std::string& dolar1);

  void literal_name_name(const std::string& dolar1, const std::string& dolar3);

  void literal_date(const std::string& dolar1);

  void literal_constant(const std::string& dolar1);

  void literal_bool(const std::string& dolar1);

  void literal_any(const std::string& dolar1);

  void literal_all(const std::string& dolar1);

};  // class driver

class driver::Ltree {
 public:
  /* ............................................... */
  /* .....ltree.........structure to represent the logical tree
  */

  std::vector<std::string> ltree;

  /* ............................................... */
  //std::vector<pair<std::string,std::string>> ltree; possivelmente será necessário alterar o codigo para esta ltree


  Ltree();
  Ltree(std::string s);

      /*
         calculates the parent
          arguments : childs index
          returns : return parrent
      */
  std::string Parent(int indice);


      /*
          left child
          arguments : fathers index
          returns : return son of the left
      */
  std::string left_child(int indice);


      /*
          left child index
          arguments : fathers index
          returns : return the left child index
      */
  int ind_left_child(int indice);

      /*
          rigth child
          arguments : fathers index
          returns : return son of the rigth
      */
  std::string right_child(int indice);

      /*
          rigth child index
          arguments : fathers index
          returns : return the rigth child index
      */
  int ind_right_child(int indice);


  std::vector<std::string> childs_aux(std::vector<std::string> v1, std::vector<std::string> v2);

      /*
          computes the list of all children
          arguments : fathers index
          returns : returns the list of all children
      */
  std::vector<std::string> childs(int ind);

  void erasechilds(int ind);

      /*
          adds a filter to the tree
          arguments : filter
          arguments : index
      */
  void add(std::string value ,int indice );

      /*
          computes the index of a filter
          arguments : filter
          return : index
      */
  int indice(std::string value);

      /*
          change the position of one filter,
          by the position of the other
          arguments : index1
          arguments : index2
      */
  void swap(int indice1,int indice2 );


      /*
          calculates the father, common to both,closer
          arguments : index1
          arguments : index2
          return : father
      */
  std::string common_ancestor( int indice1, int indice2 );

      /*
          merge the current std::vector with another
          arguments : std::vector
      */
  void merge(std::vector<std::string> vec);

      /*
          separates into two trees (AUX)
          arguments : std::vector
          arguments : index1
          arguments : index2
      */
  std::vector<std::string> split_aux(std::vector<std::string> res, int indice1, int indice2);

      /*
          separates into two trees
          arguments : index1
          arguments : index2
      */
  std::vector<std::string> split(int indice1, int indice2);


  void insere(std::vector<std::string>::iterator it_ind, std::string s);
      /*
          add a tree to the current tree
          arguments : std::vector
          arguments : current tree index
          arguments : std::vector index
      */
  void add_tree(std::vector<std::string> v, int ind, int n=0);

      /*
          search for the closest node that is an or
          arguments : parent index
          arguments : favourite child
      */
  void search_nextOR_aux(int parent, int favourite_child);

      /*
          search for the closest node that is an or
          arguments : parent index
          arguments : favourite child
      */
  int ind_Parent(int indice);

      /*
          all the parents of given node
          arguments : child index
          return : std::vector with all parents
      */
  std::vector<int> parents(int i);

  bool dependencies(int indice);

  void search_nextOR(Graph g ,int ind);

  std::string relacao_entre(std::vector<std::string> v, int s);

  void funcao(Graph g, int x=0);

  int push_aux(int ind);

  void pushLT(int ind);

  void rewrite(int ind);

  // std::string all_same_table_aux(int indice);

  // bool all_same_table(int indice);

}; // class Ltree


class driver::Graph {
 public:
  /* ..................................................... */
  /* ... */
  std::string root;

  /* .....JOIN......

         structure to save the joins
          key => foreign key
          Values => (table1 ,table2 )
       */

  std::vector< std::vector<std::string> > join;

  /* ..... GROUPBY .....

         structure to save the groupby
          (Table ,attribute )
       */
  std::vector< std::pair <std::string,std::string> > groupby;

  /* .....TABLES.....

         structure to save the tables
          key => name of the table
          Values =>
              |key => name of the filter
              |Values => filter type  [ type = 0 it is a measure   ]
                                      [ type = 1 it is a dimension ]
       */
  std::map< std::string, std::map<std::string, std::string> > tables;

  /* ......FILTER.....

         structure to save the filters
          key => name of the attribute
          Values => filter set
       */
  std::map< std::string, std::vector<std::string> > filter;

  /* ......SELECT......

         structure to save the selects
         set attributes
       */
  std::vector< std::pair<std::string,std::string> > select;

  /* ..................................................... */

  Graph clone(std::vector<std::string> v);

  void newRoot(std::string newRoot);

    /* calculates the number of attributes of table */
    /* arguments : name of a table */
    /* returns : the number of attributes */
  int num_attributes(std::string str);

    /* add a filter */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* arguments : filter type */
  void add_filter(std::string Table,std::string filter, int type);

    /* remove a filter from a table */
    /* arguments : name of a table */
    /* arguments : filter name */
  void remove_filter(std::string Table,std::string filter);

    /* to serach for a filter in table */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* return : found the filter in the table
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
  int search_filter_in_table(std::string table, std::string filter);


    /* tables that have the filter */
    /* arguments : filter name */
    /* return : set of tables */
  std::vector<std::string> search_filter(std::string filter);

    /* look for a table */
    /* arguments : name of the table */
    /* return : if you found it or not
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
  int search_table(std::string table);

    /* add a table */
    /* arguments : name of a table */
    /* arguments : filter name */
  void add_table(std::string Table, std::string filter, std::string type);

    /* remove a table */
    /* arguments : name of a table */
  void remove_table(std::string Table);

  void add_join(std::string fk,std::string table1, std::string table2);

  void remove_join(std::string fk);


    /* add a filter to the group by */
    /* arguments : name of a table */
    /* arguments : filter name */
  void add_groupby(std::string Table,std::string filter);

    /* remove a filter to the group by */
    /* arguments : name of a table */
    /* arguments : filter name */
  void remove_groupby(std::string Table,std::string filter);

    /* add a filter to std::map filters */
    /* arguments : filter name */
    /* arguments : expression */
  void add_map_filter(std::string Filter, std::string expression);

    /* remove a filter to std::map filters */
    /* arguments : name of a table */
  void remove_map_Table_filter(std::string Table);

  bool search_map_filter(std::string s);

  std::string search_map_filter2(std::string s);

  std::string search_table_filter(std::string s);
    /* remove a filter to std::map filters */
    /* arguments : name of a table */
    /* arguments : filter name */
  void remove_map_filter(std::string Table,std::string Filter);

    /* adds a table to the Selects */
    /* arguments : name of a table */
  void add_select(std::string sel, std::string alias);

    /* remove a table to the Selects */
    /* arguments : name of a table */
    /* void remove_select(std::string sel);*/

    /* whether it is measured or not */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* return : if you found it or not
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
  int is_measured(std::string Table, std::string filter);

    /* whether it is dimension or not */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* return : if you found it or not
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
  int is_dimension(std::string Table, std::string filter);

    /* add a table */
    /* arguments : name of a table */
    /* arguments : filter name */
    /*
      void add_table(std::string Table, std::string name){
          Graph[Table].insert(filter,str);
      }
    */

    /* test if two attributes are from the same table */
    /* arguments : name of the first attribute */
    /* arguments : name of the second attribute */
    /* return : whether they are from the same table or not
                [ 1 -> if they are ]
                [ 0 -> if no       ]
     */
  int isThe_same_Table(std::string attribute1, std::string attribute2);


  int isThe_same_Table_array(std::vector<std::string> v);

}; // class Graph


}  // namespace sql

#endif  // SQL_DRIVER_INCLUDE_SQL_DRIVER_H_
