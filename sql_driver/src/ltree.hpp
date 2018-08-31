/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
#ifndef SQL_DRIVER_SRC_LTREE_H_
#define SQL_DRIVER_SRC_LTREE_H_
#include <string>
#include <vector>
#include "include/sql_driver.hpp"

namespace sql {

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

}; // class Ltree

}  // namespace sql

#endif  // SQL_DRIVER_SRC_LTREE_H_
