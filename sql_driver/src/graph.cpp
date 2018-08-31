/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
#include "src/graph.hpp"

namespace sql {

driver::Graph
driver::Graph::clone(std::vector<std::string> v) {
  driver::Graph newgraph;
  //root
  //join
  //retirar de filter os filtros
  newgraph.root = root;
  newgraph.join = std::vector<std::vector<std::string> >(join);
  // newgraph.tables = std::map<std::string,std::map<std::string,std::string> >(tables);

  std::map<std::string, std::vector<std::string> > f;
  std::map<std::string,std::map<std::string,std::string> > t;
  std::vector<std::pair <std::string,std::string> > g;
  std::vector<std::string> notEmpty;

  for(std::map<std::string,std::vector<std::string> >::iterator it = filter.begin(); it != filter.end(); ++it) {
    std::vector<std::string> aux;
    for(std::vector<std::string>::iterator i = (it->second).begin(); i != (it->second).end(); ++i) {
      if (has(v, *i)) {
        aux.push_back(*i);
      }
    }
    if (!aux.empty()) {
      notEmpty.push_back(it->first);
      f.insert(std::pair<std::string, std::vector<std::string> >((it->first), aux));
    }
  }

  newgraph.filter = f;

  for(std::map<std::string,std::map<std::string, std::string> >::iterator it = tables.begin(); it != tables.end(); ++it) {
    std::map<std::string, std::string> aux;
    for(std::map<std::string, std::string>::iterator i = (it->second).begin(); i != (it->second).end(); ++i) {
      if (has(notEmpty, i->first) || has(v,i->first)) {
        aux.insert(std::pair<std::string, std::string> (i->first, i->second));
      }
    }
    t.insert(std::pair<std::string, std::map<std::string, std::string> > ((it->first), aux));
  }

  newgraph.tables= t;

  for(std::vector<std::pair<std::string, std::string> >::iterator it = groupby.begin(); it != groupby.end(); ++it) {
    //if (has(notEmpty,it->second)) {
    g.push_back(*it);
    //}
  }
  newgraph.groupby = g;
  return newgraph;
}

void driver::Graph::newRoot(std::string newRoot){
    root = newRoot;
}
    /* calculates the number of attributes of table */
    /* arguments : name of a table */
    /* returns : the number of attributes */

int driver::Graph::num_attributes(std::string str){
    return tables[str].size();
}

    /* add a filter */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* arguments : filter type */
/*
void driver::Graph::add_filter(std::string Table,std::string filter, int type ){
    tables[Table].insert(std::pair<std::string,int>(filter,type));
}
*/
    /* remove a filter from a table */
    /* arguments : name of a table */
    /* arguments : filter name */
/*
void driver::Graph::remove_filter(std::string Table,std::string filter){
    tables[Table].erase(filter);
}
*/
    /* to serach for a filter in table */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* return : found the filter in the table
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
int driver::Graph::search_filter_in_table(std::string table, std::string filter){
  for(std::map<std::string,std::string>::iterator it = tables[table].begin(); it != tables[table].end(); ++it) {
    if (it->first.compare(filter)==0) {return 1;}
  }
  return 0;
}


    /* tables that have the filter */
    /* arguments : filter name */
    /* return : set of tables */
std::vector<std::string> driver::Graph::search_filter(std::string filter){
  std::vector<std::string> aux;
  for(std::map<std::string, std::map<std::string,std::string> >::iterator it = tables.begin(); it != tables.end(); ++it) {
    if (search_filter_in_table(it->first,filter)) {
      std::cout << "iguais" << std::endl;
      aux.push_back(it->first);
    }
  }
  return aux;
}

    /* look for a table */
    /* arguments : name of the table */
    /* return : if you found it or not
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
int driver::Graph::search_table(std::string table){
  for(std::map<std::string, std::map<std::string,std::string> >::iterator it = tables.begin(); it != tables.end(); ++it) {
    if (it->first.compare(table)==0) {return 1;}
  }
  return 0;
}

    /* add a table */
    /* arguments : name of a table */
    /* arguments : filter name */

void driver::Graph::add_table(std::string Table, std::string filter, std::string type){
  std::map<std::string,std::string> aux = tables[Table];
  aux[filter] = type;
  tables[Table] = aux;
}


    /* remove a table */
    /* arguments : name of a table */
/*
void driver::Graph::remove_table(std::string Table){
    tables.erase(Table);
}
*/

void driver::Graph::add_join(std::string fk,std::string table1, std::string table2){
  std::vector<std::string> aux ;
  aux.push_back(fk);
  aux.push_back(table1);
  aux.push_back(table2);
  join.push_back(aux);
}

//        // join[Table] é um par
//        void driver::Graph::remove_join(std::string fk){
//            join.erase(fk);
//        }


    /* add a filter to the group by */
    /* arguments : name of a table */
    /* arguments : filter name */
void driver::Graph::add_groupby(std::string Table,std::string filter){
  groupby.push_back(std::pair<std::string,std::string>(Table,filter));
}

    /* remove a filter to the group by */
    /* arguments : name of a table */
    /* arguments : filter name */
void driver::Graph::remove_groupby(std::string Table,std::string filter){
  for (int i = 0; i < groupby.size(); i++) {
    if ((groupby[i].first.compare(Table)==0) && (groupby[i].second.compare(filter)==0)) {
      groupby.erase(groupby.begin() + i);
      break;
    }
  }
}

    /* add a filter to std::map filters */
    /* arguments : filter name */
    /* arguments : expression */
void driver::Graph::add_map_filter(std::string Filter, std::string expression){
  filter[ Filter ].push_back(expression);
}

bool driver::Graph::search_map_filter(std::string s){
  for(std::map<std::string,std::vector<std::string> >:: iterator it = filter.begin(); it != filter.end() ; ++it){
    if(it->first.compare(s) == 0){
      return true;
    }
  }
  return false;
}

std::string driver::Graph::search_map_filter2(std::string s){
  for(std::map<std::string,std::vector<std::string> >:: iterator it = filter.begin(); it != filter.end() ; ++it){
    for(std::vector<std::string>:: iterator it2 = it->second.begin(); it2 != it->second.end() ; ++it2){
      if(it2->compare(s) == 0){
        return it->first;
      }
    }
  }
  return s;
}

std::string driver::Graph::search_table_filter(std::string s){
  std::string aux = search_map_filter2(s);
  std::vector<std::string> aux2 = search_filter(aux);
  return aux2[0];
}

    /* remove a filter to std::map filters */
    /* arguments : name of a table */
void driver::Graph::remove_map_Table_filter(std::string Table){
  filter.erase(Table);
}

    /* remove a filter to std::map filters */
    /* arguments : name of a table */
    /* arguments : filter name */
void driver::Graph::remove_map_filter(std::string Table,std::string Filter){
  for (int i = 0; i < filter[Table].size(); i++) {
    if (filter[Table][i].compare(Filter)==0){
      filter[Table].erase(filter[Table].begin() + i);
      break;
    }
  }
}

    /* adds a table to the Selects */
    /* arguments : name of a table */
void driver::Graph::add_select(std::string sel, std::string alias){
  select.push_back(std::pair<std::string,std::string>(sel,alias));
}

    /* remove a table to the Selects */
    /* arguments : name of a table */
/*
void driver::Graph::remove_select(std::string Table){
    for (int i = 0; i < select.size(); i++) {
        if (select[i].compare(Table)==0) {
            select.erase(select.begin() + i);
            break;
        }
    }
}
*/
    /* whether it is measured or not */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* return : if you found it or not
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
int driver::Graph::is_measured(std::string Table, std::string filter) {
  if (tables[Table][filter].compare("measure") == 0) {
    return 1;
  } else {
    return 0;
  }
}

    /* whether it is dimension or not */
    /* arguments : name of a table */
    /* arguments : filter name */
    /* return : if you found it or not
                [ 1 -> found himself ]
                [ 0 -> if not found  ]
     */
int driver::Graph::is_dimension(std::string Table, std::string filter) {
  if (tables[Table][filter].compare("dimension") == 0) {
    return 1;
  } else {
    return 0;
  }
}
    /* add a table */
    /* arguments : name of a table */
    /* arguments : filter name */
/*
void driver::Graph::add_table(std::string Table, std::string name){
    driver::Graph[Table].insert(filter,str);
}
*/

    /* test if two attributes are from the same table */
    /* arguments : name of the first attribute */
    /* arguments : name of the second attribute */
    /* return : whether they are from the same table or not
                [ 1 -> if they are ]
                [ 0 -> if no       ]
     */
//        int driver::Graph::isThe_same_Table(std::string attribute1, std::string attribute2){
//            std::vector<std::string> v1 = search_filter(attribute1);
//            std::vector<std::string> v2 = search_filter(attribute2);
//            std::vector<std::string>::iterator it;
//            std::vector<std::string> inter(max(v1.size(),v2.size()));
//
//            it = set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),inter.begin());
//            if (inter.size() > 0) {
//                return 1;
//            }
//            else return 0;
//        }

int driver::Graph::isThe_same_Table(std::string attribute1, std::string attribute2){
  if ( relacao_entre_arrays(search_filter(attribute1),search_filter(attribute2))=="NULL") {
    return 0;
  } else {
    return 1;
  }
}

int driver::Graph::isThe_same_Table_array(std::vector<std::string> v){
  int res = 1;
  for(std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
    if (!isThe_same_Table(v[0],*it)) {
      res = 0;
      break;
    }
  }
  return res;
}

} // namespace sql
