/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
#ifndef SQL_DRIVER_SRC_GRAPH_H_
#define SQL_DRIVER_SRC_GRAPH_H_
#include <map>
#include <string>
#include <vector>
#include "include/sql_driver.hpp"

namespace sql {

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

#endif  // SQL_DRIVER_SRC_GRAPH_H_
