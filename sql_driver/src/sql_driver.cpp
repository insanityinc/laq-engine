/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
#include <regex>
#include <string>
#include <vector>
#include "include/sql_driver.hpp"

namespace sql {

driver::Ltree::Ltree(){
            ltree = std::vector<std::string>();
        }

        driver::Ltree::Ltree(std::string s){
            ltree = std::vector<std::string>();
            ltree.push_back(s);
        }

            /*
               calculates the parent
                arguments : childs index
                retuens : return parrent
            */
        std::string driver::Ltree::Parent(int indice){
            std::string res = "NULL";
            int aux = (indice-1)/2;
            if ( aux <0 || aux >= ltree.size()) {
                return res;
            }else{ return ltree[aux]; }
            }


            /*
                left child
                arguments : fathers index
                retuens : return son of the left
            */
        std::string driver::Ltree::left_child(int indice){
            std::string res = "NULL";
            int aux = 2*indice+1;
            if ( aux <0 || aux >= ltree.size()) {
                return res;
            }else{ return ltree[aux]; }
            }


            /*
                left child index
                arguments : fathers index
                retuens : return the left child index
            */
        int driver::Ltree::ind_left_child(int indice){
            int aux = 2*indice+1;
            if ( aux <0 || aux >= ltree.size()) {
                return aux;
            }else{ return aux; }
            }

            /*
                rigth child
                arguments : fathers index
                retuens : return son of the rigth
            */
        std::string driver::Ltree::right_child(int indice){
            std::string res = "NULL";
            int aux = 2*indice+2;
            if ( aux <0 || aux >= ltree.size()) {
                return res;
            }else{ return ltree[aux]; }
            }

            /*
                rigth child index
                arguments : fathers index
                retuens : return the rigth child index
            */
        int driver::Ltree::ind_right_child(int indice){
            int aux = 2*indice+2;
            if ( aux <0 || aux >= ltree.size()) {
                return aux;
            }else{ return aux; }
            }

            /*
                computes the list of all children
                arguments : fathers index
                retuens : returns the list of all children
            */
        std::vector<std::string> driver::Ltree::childs_aux(std::vector<std::string> v1, std::vector<std::string> v2){
          for(int x = 0; x < v2.size(); x++){
            v1.push_back(v2[x]);
          }
          return v1;
        }


        std::vector<std::string> driver::Ltree::childs(int ind){
               std::vector<std::string> res;
               std::string aux = right_child(ind);
               if (aux.compare("NULL") != 0) {
                   res.push_back(aux);
                   res = childs_aux(res,childs(ind_right_child(ind)));
               }
               aux = left_child(ind);
               if (aux.compare("NULL") != 0) {
                   res.push_back(aux);
                   res = childs_aux(res,childs(ind_left_child(ind)));
               }
               return res;
           }



        void driver::Ltree::erasechilds(int ind){
            ltree.reserve(1+ind);
            if (ltree[ind].compare("NULL") != 0) {
                ltree[ind] = "NULL";
                erasechilds(ind_left_child(ind));
                erasechilds(ind_right_child(ind));
            }
        }
            /*
                adds a filter to the tree
                arguments : filter
                arguments : index
            */
        void driver::Ltree::add(std::string value ,int indice ){
          int x = ltree.size();
          while(x<=indice){
              ltree.push_back("NULL");
              x++;
          }
          ltree[indice] = value;
        }

            /*
                computes the index of a filter
                arguments : filter
                return : index
            */
        int driver::Ltree::indice(std::string value){
           for ( int i = 0; i < ltree.size(); i++ ) {
               if (!ltree[i].compare(value)) {
                   return i;
               }
           }
           return -1;
        }

            /*
                change the position of one filter,
                by the position of the other
                arguments : index1
                arguments : index2
            */
        void driver::Ltree::swap(int indice1,int indice2 ){
            std::string aux;
            ltree.reserve(1 + std::max(indice1,indice2));
            aux = ltree[indice1];
            ltree[indice1] = ltree[indice2];
            ltree[indice2] = aux;
        }


            /*
                calculates the father, common to both,closer
                arguments : index1
                arguments : index2
                return : father
            */
        std::string driver::Ltree::common_ancestor( int indice1, int indice2 ){
            ltree.reserve(1 + std::max(indice1,indice2));
            std::vector<int> parent_1 = parents(indice1);
            std::vector<int> parent_2 = parents(indice2);
            for ( int i = 0;i < parent_1.size(); i++) {
              for ( int j = 0; j < parent_2.size();j++)
                if (parent_1[i] == parent_2[j]) {
                    return(ltree[parent_1[i]]);
                }
            }
            return "NULL";
        }

            /*
                merge the current std::vector with another
                arguments : std::vector
            */
        void driver::Ltree::merge(std::vector<std::string> vec){
            for ( int i = 0; ltree.size(); i++) {
                if ((!ltree[i].compare("NULL")) && (vec[i].compare("NULL")==0)) {
                    ltree[i] = vec[i];
                }
            }
        }

            /*
                separates into two trees (AUX)
                arguments : std::vector
                arguments : index1
                arguments : index2
            */
        std::vector<std::string> driver::Ltree::split_aux(std::vector<std::string> res, int indice1, int indice2){
            if (!ltree[indice2].compare("NULL")) {
                return res;
            }
            res[indice1] = ltree[indice2];
            std::vector<std::string> aux1 = driver::Ltree::split_aux(res,ind_left_child(indice1),ind_left_child(indice2));
            std::vector<std::string> aux2 = driver::Ltree::split_aux(res,ind_right_child(indice1),ind_right_child(indice2));
            //return aux1.merge(aux2);
            return res;
        }

            /*
                separates into two trees
                arguments : index1
                arguments : index2
            */
        std::vector<std::string> driver::Ltree::split(int indice1, int indice2){
            std::vector<std::string> res;
            ltree.reserve(1 + std::max(indice1,indice2));
            return  driver::Ltree::split_aux(res,indice1,indice2);
        }

            /*
                add a tree to the current tree
                arguments : std::vector
                arguments : current tree index
                arguments : std::vector index
            */
        void driver::Ltree::add_tree(std::vector<std::string> v, int ind, int n){
          if(n < v.size()){
                while(ltree.size()<=ind){
                  ltree.push_back("NULL");
                }
                ltree.at(ind)=v[n];
                if (v[n].compare("NULL") != 0) {
                  add_tree(v,ind_left_child(ind),ind_left_child(n));
                  add_tree(v,ind_right_child(ind),ind_right_child(n));
                }
            }
        }

            /*
                search for the closest node that is an or
                arguments : parent index
                arguments : favourite child
            */
            /*
        void driver::Ltree::search_nextOR_aux(int parent, int favourite_child){
            if (favourite_child) {
               swap(parent,ind_right_child(parent));
               add("NULL",ind_left_child(parent));
               add("NULL",ind_right_child(parent));
            }else{
               swap(parent, ind_left_child(parent));
               add("NULL",ind_left_child(parent));
               add("NULL",ind_right_child(parent));
                }
        }
*/



/*
     std::string driver::Ltree::relacao_entre(std::vector<std::string> v, int s){
         std::vector<int> aux = parents(s);
         int i = 0;
         for(std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
             aux = substd::vector(aux,parents(i));
             i++;
         }
         if (aux.size()) {
             return ltree[aux[0]];
         }else {return "NULL";}//nunca acontece em teoria mas quem sabe
     }

*/
        /*
            search for the closest node that is an or
            arguments : parent index
            arguments : favourite child
        */
    int driver::Ltree::ind_Parent(int indice){
        int aux = (indice-1)/2;
        if ( aux <0 || aux >= ltree.size()) {
            return -1;
        }else{ return aux; }
    }
        /*
            all the parents of given node
            arguments : child index
            return : std::vector with all parents
        */
    std::vector<int> driver::Ltree::parents(int i){
       std::vector<int> res;
//       std::vector<int>::iterator it;
 //      it = res.begin();
       for (int ind = i;ind_Parent(ind)>=0;i = ind_Parent(ind)) {
           //res.insert(i);
           res.push_back(ind);
       }
       return res;
    }

    bool driver::Ltree::dependencies(int indice){
      ltree.reserve(1+indice);
      if(ltree[indice].compare("AND") == 0){
        return dependencies(ind_left_child(indice)) && dependencies(ind_right_child(indice));
      }
      if(ltree[indice].compare("OR") == 0){
        return all_same_table(indice);
      }
      return true;
    }
/*
    void driver::Ltree::search_nextOR(driver::Graph g ,int ind){
      if (left_child(ind_left_child(ind)) == "OR" && (g.isThe_same_Table_array(childs(ind))==0)){
            search_nextOR_aux(ind_left_child(ind_left_child(ind)),0);
            search_nextOR_aux(ind_right_child(ind_left_child(ind)),1);
      }else{
          if (right_child(ind_left_child(ind)) == "OR" && (g.isThe_same_Table_array(childs(ind))==0)){
            search_nextOR_aux(ind_left_child(ind_left_child(ind)),0);
            search_nextOR_aux(ind_right_child(ind_right_child(ind)),1);
          }
      }

    }


    void driver::Ltree::funcao(driver::Graph g, int x){
      if(ltree[x] == "OR" && (g.isThe_same_Table_array(childs(x))==0)){
            int parent = ind_Parent(x);
                std::vector<std::string> s = split(0,parent);
                add_tree(s,ind_left_child(x),0);
                add_tree(s,ind_right_child(x),0);
                add("OR",parent);
                search_nextOR(g,parent);
      }else{
        if(ltree[x]!="NULL"){
          funcao(g,(ind_left_child(x)));
          funcao(g,(ind_right_child(x)));
        }
      }
    }
*/
     int driver::Ltree::push_aux(int ind){
        ltree.reserve(1+ind);
         if (left_child(ind)!= "NULL"){
             int child = ind_left_child(ind);
             swap(child,ind);
             push_aux(child);
         }
         if (right_child(ind)!= "NULL"){
             int child = ind_right_child(ind);
             swap(child,ind);
             push_aux(child);
         }
         return ind;
     }

    void driver::Ltree::pushLT(int ind){
      ltree.reserve(1+ind);
      swap(ind,ind_Parent(ind));
      int last = push_aux(ind);
       if((ltree[last] =="AND" || ltree[last] =="OR")){
      ltree.erase(ltree.begin() + last);
       }
    }


     void driver::Ltree::rewrite(int ind){
       ltree.reserve(1+ind);
       if((ltree[ind]=="AND" || ltree[ind]=="OR")){
         if (left_child(ind)== "NULL"){
           pushLT(ind_right_child(ind));
         }
         if(right_child(ind) == "NULL"){
           pushLT(ind_left_child(ind));
         }
       }
       if(ltree[ind]!="NULL"){
         rewrite(ind_left_child(ind));
         rewrite(ind_right_child(ind));
         }
    }

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

driver::driver() : trace_scanning(false), trace_parsing(false) {
  mainGraph = new Graph();
  g = new Graph();
  l = new Ltree();
  itr = 0;
  itr2 = 0;
}

driver::~driver() {
  delete mainGraph;
  delete g;
  delete l;
}

int
driver::parse(const std::string &f) {
  file = f;
  if (f.length() <= 4) {
    error("Error: Filename too short");
    return 1;
  }
  if (f.substr(f.length() - 4) != ".sql") {
    error("Error: SQL file expected");
    return 2;
  }
  scan_begin();
  yy::sql_parser parser(this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end();
  return res;
}

void
driver::error(const yy::location& l, const std::string& m) {
  std::cerr << l << ": " << m << std::endl;
}

void
driver::error(const std::string& m) {
  std::cerr << m << std::endl;
}


bool
has (std::vector<std::string> v, std::string s) {
  return (std::find(v.begin(), v.end(), s) != v.end());
}



    driver::Ltree create_tree(std::string term, std::string type){
        driver::Ltree res ;
        if (type.compare("JOIN") == 0) {
            std::cout << "join" << std::endl;
            res.add("NULL",0);
        }else{
            res.add(term,0);
            res.add("NULL",1);
            res.add("NULL",2);
        }
        return res;
    }

    driver::Ltree join_trees(driver::Ltree l1, driver::Ltree l2, std::string type){
        driver::Ltree res;
        res.add(type,0);
        res.add_tree(l1.ltree, 1,0);
        res.add_tree(l2.ltree, 2,0);
        return res;
    }

    std::string getTable(std::string attribute) {
        int ind = attribute.find(".");
        //if (ind != (-1)) {
        //    attribute = attribute.substr(ind);
        //}
        std::vector<std::string> v ( mainGraph->search_filter(attribute));
        if (v.size() < 1) {
            return "";
        }else{
            std::cout << v[0] << "aqui" << "\n";
            return v[0];
        }
    }

     std::vector<int> subvector (std::vector<int> v1, std::vector<int> v2){
         std::vector<int> res(max(v1.size(),v2.size()));
         std::vector<int>::iterator aux;
         aux = set_intersection (v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin());
         res.resize(aux-res.begin());
         return res;
     }

    std::string relacao_entre(std::vector<std::string> v, std::string s){
        std::vector<int> aux = l.parents(l.indice(s));
        for(std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
            aux = subvector(aux,l.parents(l.indice(*it)));

        }

        if (aux.size()) {
            int x = aux[0];
            l.ltree.reserve(1+x);
            return l.ltree[x];
        }else {return "NULL";}
    }

    std::string relacao_entre_arrays(std::vector<std::string> v, std::vector<std::string> s){
        for(std::vector<std::string>::iterator it = v.begin()+1; it != v.end(); ++it) {
            s.push_back(*it);
        }
        return relacao_entre(s,v[0]);
    }


void next(){
  if(a.compare("")==0){
    a = "A";
  }else{
    if(a[a.length()-1] == 'Z'){
      a.append("A");
    }else{
      a[a.length()-1]++;
    }
  }
}

void joinAtrD(std::vector<std::string> v, std::string type){
  if(v.size()>=1){
    next();
    std::cout << a << " = filter(" << v[0];
    for(std::vector<std::string>::iterator it = v.begin()+1; it != v.end(); ++it) {
      std::cout << " " << type << " " << *it;
    }
    std::cout << ")\n";
  }
}

void joinAtr(std::string start, std::string type){
  //pega nos filtros no mesmo atributo
  //std::map<std::string,int> work = g.tables[start];
  std::map<std::string,std::string> work( g.tables[start] );
  std::vector<std::string> v;
  //std::vector<std::string,int> new_elements;
  std::map<std::string,std::string> new_elements;
  for(std::map<std::string,std::string>::iterator it = work.begin(); it != work.end(); ++it) {
    std::string s = it->first;
    //std::vector<std::string> v = g.filter[s];
    if((it->second).compare("measure") == 0 || s.find(".")==std::string::npos){ //se for medida
      new_elements.insert(pair<std::string,std::string>(it->first,it->second));
    }else{
      std::vector<std::string> v ( g.filter[s] );
      joinAtrD(v,type);
      new_elements.insert(pair<std::string,std::string>(a,it->second));
    }
  }
  g.tables[start] = new_elements;
}


void joinFiltersD(std::vector<std::string> v, std::string type){
  next();
  std::string s;
  if(type.compare("AND") == 0){
    s = "krao";
  }else{
    s = "kraoOR";
  }
  std::cout << a << " = "<< s <<"(" << v[0] << ", " << v[1] << ")\n";
  for(std::vector<std::string>::iterator it = v.begin()+2; it != v.end(); ++it) {
    std::string aux = a;
    next();
    std::cout << a << " = " << s <<"(" << aux << ", " << *it << ")\n";
  }
}

void joinFilters(std::string start, std::string type){
  //pega nos filtros da mesma tabela
  std::string aux;
  std::string aux2;
  std::map<std::string,std::string> work (g.tables[start]);
  std::vector<std::string> medidas;
  std::vector<std::string> dimensoes;
  for(std::map<std::string,std::string>::iterator it = work.begin(); it != work.end(); ++it) {
    std::string s = it->first;
    if((it->second).compare("measure") == 0){ //se for medida
      std::vector<std::string> filters= g.filter[s];
      for(std::vector<std::string>::iterator it = filters.begin(); it != filters.end(); ++it){
        medidas.push_back(*it);
      }
    }else{                //se for dimensao
      dimensoes.push_back(s);
    }
  }
  joinAtrD(medidas,type);
  aux = a;
  if(dimensoes.size() > 0){
    if( dimensoes.size() == 1){
      aux2 = dimensoes[0];
    }else{
      joinFiltersD(dimensoes,type);
      aux2 = a;
    }
  }
  if(dimensoes.size()>0 && medidas.size()>0){
    next(); // verificar se é and ou or entre aux1 e aux2
    if(type.compare("AND") == 0){
      std::cout << a << " = " << "krao" << "(" << aux << ", " << aux2 << ")\n";
    }else{
      std::cout << a << " = " << "kraoOR" << "(" << aux << ", " << aux2 << ")\n";
    }
  }
  if(dimensoes.size()>0){
    std::map<std::string,std::string> t;
    t[a]="dimension";
    g.tables[start] = t;
  }else{
    if(medidas.size()>0){
      std::map<std::string,std::string> t;
      t[a]="measure";
      g.tables[start] = t;
    }
  }
}


void joinGroupbyAux(std::string gb, std::string start){
  std::string alpha = a;
  next();
  std::cout << a << " = krao(" << gb << "," << alpha << ")\n";
  std::map<std::string,std::string> work;
  work[a]="dimension";
  g.tables[start]=work;
}

void joinGroupby(std::string start){
  if(g.groupby.size()>0 && g.tables[start].size()>0){
    for(std::vector<pair<std::string,std::string> >::iterator it = g.groupby.begin(); it != g.groupby.end(); ++it) {
      if((it->first).compare(start) == 0){
        joinGroupbyAux(it->second,start);
      }
    }
  }
}

void removeTable(std::string start){
  std::vector<std::vector <std::string> > Keys;
  std::string alpha = a;
  if (g.tables[start].size() <= 1) {
    for(std::vector<std::vector<std::string> >::iterator it = g.join.begin(); it != g.join.end(); ++it) {
      if((it->at(2)).compare(start) == 0){
        if(g.tables[start].size() > 0){
          next();
          std::cout << a << " = " << "dot(" << alpha << ", " << it->at(0)<< ")\n";
          std::map<std::string,std::string> aux = g.tables[it->at(1)];
          aux[a] = "dimension";
          g.tables[it->at(1)] = aux;
        }
      }else{
        Keys.push_back(*it);
      }
    }
    g.join = Keys;
    std::map<std::string,std::string> t;
    g.tables[start] = t;
  }
}

std::string giveMeStart(std::string root){
  if(g.join.size() > 0){
    for(std::vector<std::vector<std::string> >::iterator it = g.join.begin(); it != g.join.end(); ++it) {
      if((it->at(1)).compare(root) == 0){
        //std::cout << it->at(1) << "\n";
        return giveMeStart(it->at(2));
      }
    }
  }
  return root;
}

std::string giveMeFinish(std::string root, std::vector<std::string> v){
  if(mainGraph->join.size() > 0){
    for(std::vector<std::vector<std::string> >::iterator it = mainGraph->join.begin(); it != mainGraph->join.end(); ++it) {
      if(has(v,it->at(1)) && !(has(v,it->at(2)))){
        return it->at(2);
      }
    }
  }
  return root;
}

void print_tables3(){
  std::cout << "======================" << "\n";
  for(std::map<std::string,std::map<std::string,std::string> >::iterator it = g.tables.begin(); it != g.tables.end(); ++it) {
    std::cout << it->first << ":\n";
    for(std::map<std::string,std::string>::iterator it2 = g.tables[it->first].begin(); it2 != g.tables[it->first].end(); ++it2) {
      std::cout << it2->first <<"\n";
    }
  }
  std::cout << "--------------------" << "\n";
}
int driver::full_num_attributes(){
  int res = 0;
  for(std::map<std::string,std::map<std::string,std::string> >::iterator it = g.tables.begin(); it != g.tables.end(); ++it) {
    res += g.num_attributes(it->first);
  }
  return res;
}



void driver::graphWorkAux(std::string type){
  //pega nas pontas
  std::string start = giveMeStart(g.root);
  //juntar filtros do mesmo atributo dessa tabela, no final a tabela terá aoenas letras.
  joinAtr(start, type);
  //juntar tudo
  joinFilters(start, type);
  //juntar groupbys
  //joinGroupby(start, type);
  joinGroupby(start);
  //remover tabela e adicionar como atributo às seguintes
  if(start.compare(g.root)!=0)removeTable(start);
}

void driver::graphWork(std::string type){
  while(full_num_attributes() > 1){
    graphWorkAux(type);
  }

}




//void trabalha(int ind){
//  trabalhaaux(l.ind_left_child(ind),std::string tabela1);
//  trabalhaaux(l.ind_right_child(ind),std::string tabela2);
//}
//
//void trabalhaaux(int ind, int count){
//    if(is_the_same_table_array(childs(ind)) == 0 )
//      if(left_child(ind) == "AND" || right_child(ind) == "AND")
//        trabalhaaux(int ind);
//}
/*
void cleanexp(){
current_expression.clear();
current_expression2.clear();
}

void addexp(std::string exp){
if (current_expression.empty()){
    current_expression = exp;
    return;
}
current_expression2 = exp;
}

void aux(std::string exp){
if (current_expression2.empty()){
    mainGraph->add_std::map_filter( current_expression, exp);
}else {
    mainGraph->add_join(current_expression2,getTable(current_expression),getTable(current_expression2));
}
}
*/


void driver::print_tables2(){
  std::cout << "======================" << "\n";
  for(std::map<std::string,std::map<std::string,std::string> >::iterator it = mainGraph->tables.begin(); it != mainGraph->tables.end(); ++it) {
    std::cout << it->first << ":\n";
    for(std::map<std::string,std::string>::iterator it2 = mainGraph->tables[it->first].begin(); it2 != mainGraph->tables[it->first].end(); ++it2) {
      std::cout << it2->first <<"\n";
    }
  }
  std::cout << "--------------------" << "\n";
}

void driver::merge(std::vector<std::string> v){
// g
// mainGraph
  std::pair<std::string,std::string> aux;
  std::string table;
  for(std::map<std::string,std::map<std::string, std::string> >::iterator it = g->tables.begin(); it != g->tables.end(); ++it) {
    if (!((it->second).empty())) {
        for(std::map<std::string, std::string> ::iterator i = (it->second).begin(); i != (it->second).end(); ++it) {
            aux = std::pair<std::string,std::string>(i->first,i->second);
            table = it->first;
            break;
        }
        break;
      }
  }

  mainGraph->tables[table].insert(aux);
  //empty table
  std::vector<std::string> emptyfilter;

  std::map<std::string, std::vector<std::string> > newfilter;
  for(std::map<std::string,std::vector<std::string> >::iterator it = mainGraph->filter.begin(); it != mainGraph->filter.end(); ++it) {
      std::vector<std::string> aux;
      for(std::vector<std::string>::iterator i = (it->second).begin(); i != (it->second).end(); ++i) {
          if (!has(v, *i)) {
              aux.push_back(*i);
          }
      }
      if (!aux.empty()) {
          newfilter.insert(std::pair<std::string,std::vector<std::string> >(it->first,aux));
      }else{ emptyfilter.push_back(it->first);}
      //acrescenta no empty
      }
      mainGraph->filter = newfilter;
      std::vector<std::pair<std::string,std::string> > eraseIt;

  for(std::map<std::string,std::map<std::string, std::string> >::iterator it = mainGraph->tables.begin(); it != mainGraph->tables.end(); ++it) {
     // se it->second->first in empty elimina std::map<std::string,std::string>
      for(std::map<std::string, std::string>::iterator i = (it->second).begin(); i != (it->second).end(); ++i) {
          if (has(emptyfilter, i->first) || has(v,i->first)) {
              eraseIt.push_back(make_pair(it->first, i->first));
          }
      }
  }
  for(std::vector<std::pair<std::string,std::string> >::iterator it = eraseIt.begin(); it != eraseIt.end(); ++it) {
      mainGraph->tables[it->first].erase(it->second);
  }
}
/*
void dot_all(){
  for(std::vector<std::vector<std::string> >::iterator it = mainGraph->join.begin(); it != mainGraph->join.end(); ++it) {
    std::string key = it->at(0);
    std::string table = it->at(1);
    std::string table2 = it->at(2);
    std::map<std::string,std::string> aux = mainGraph->tables[table];
    for(std::map<std::string,std::string>::iterator it = mainGraph->tables[table2].begin(); it != mainGraph->tables[table2].end(); ++it) {
      next();
      std::cout << a << " = " << "dot(" << it->first << ", " << key << ")\n";
      aux[a] = "dimension";
    }
    mainGraph->tables[table] = aux;
  }
}*/
void driver::print_tables(){
  std::cout << "======================" << "\n";
  for(std::map<std::string,std::map<std::string,std::string> >::iterator it = g->tables.begin(); it != g->tables.end(); ++it) {
    std::cout << it->first << ":\n";
    for(std::map<std::string,std::string>::iterator it2 = g->tables[it->first].begin(); it2 != g->tables[it->first].end(); ++it2) {
      std::cout << it2->first <<"\n";
    }
  }
  std::cout << "--------------------" << "\n";
}


void driver::print_tree(){
  for(int x = 0; x<l->ltree.size(); x++){
    std::cout << l->ltree[x] << "\n";
  }
  std::cout << "\n";
}

void driver::dot_all_aux(std::string start){
  std::string alpha = a;
  if (mainGraph->tables[start].size() <= 1) {
    for(std::vector<std::vector<std::string> >::iterator it = mainGraph->join.begin(); it != mainGraph->join.end(); ++it) {
      if((it->at(2)).compare(start) == 0){
        if(mainGraph->tables[start].size() > 0){
          std::map<std::string,std::string> aux = mainGraph->tables[it->at(1)];
          for(std::map<std::string,std::string>::iterator it2 = mainGraph->tables[start].begin(); it2 != mainGraph->tables[start].end(); ++it2){
            if(!mainGraph->search_map_filter(it2->first)){
              next();
              std::cout << a << " = " << "dot(" << it2->first << ", " << it->at(0) << ")\n";
              aux[a] = "dimension";
              l->ltree[l->indice(it2->first)] = a;
            }else{
              for(std::vector<std::string>::iterator it3 = mainGraph->filter[it2->first].begin(); it3 != mainGraph->filter[it2->first].end(); ++it3){
                next();
                std::cout << a << " = " << "dot(" << *it3 << ", " << it->at(0) << ")\n";
                aux[a] = "dimension";
                l->ltree[l->indice(*it3)] = a;
              }
            }
          }
          mainGraph->tables[it->at(1)] = aux;
          mainGraph->tables[it->at(2)].clear();
        }
      }
    }
  }
}


void driver::dot_all(){
  std::vector<std::string> aux;
  aux.push_back(mainGraph->root);
  std::string start;
  for(int x = 0; x < mainGraph->tables.size(); x++){
    start = giveMeFinish(mainGraph->root,aux);
    dot_all_aux(start);
    aux.push_back(start);
  }
}
std::vector<std::string> driver::resolveS_aux(std::vector<std::string> v){
  std::vector<std::string> aux;
  for(int x = 0; x< v.size(); x++){
    if((v[x].compare("AND")!=0) || (v[x].compare("OR")!=0)){
      aux.push_back(v[x]);
    }
  }
  return aux;
}



void driver::resolveS(int indice, std::string type){
  std::vector<std::string> v =  l->childs(indice);
  (*g) = mainGraph->clone(resolveS_aux(v));
  graphWork(type);
  l->ltree.reserve(1+indice);
  l->ltree[indice] = a;
  l->erasechilds(l->ind_left_child(indice));
  l->erasechilds(l->ind_right_child(indice));
  merge(v); //between mainGraph and g...
//^^cuidado com vários filters no memso atributo, retirar de table e filter apenas se g.filter(atributo)==mainGraph->filter(atributo), caso contrário apenas retira as entradas iguais
}
void driver::print_trees(){
  for(int x = 0; x<trees.size(); x++){
    for(int y = 0; y<trees[x].ltree.size(); y++){
      std::cout << trees[x].ltree[y] << "\n";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}


void driver::resolve(int indice){
  l->ltree.reserve(1+indice);
  if(l->ltree[indice].compare("OR") == 0){
    resolve(l->ind_left_child(indice));
    resolve(l->ind_right_child(indice));
    if(l->dependencies(indice) == true){
      resolveS(indice,"OR");
    }else{
      dot_all();
      resolve(indice);
    }
  }
  if(l->ltree[indice].compare("AND") == 0){
    if(has(l->childs(indice),"OR")){
      resolve(l->ind_left_child(indice));
      resolve(l->ind_right_child(indice));
      resolveS(indice,"AND");
    }else{
      resolveS(indice,"AND");
    }
  }
}

void driver::dot_if_needed(){
  while(mainGraph->num_attributes(mainGraph->root)!= 1){
    dot_all();
  }
}


void driver::returnf(){
  std::vector<std::string> aux;
  std::vector<std::string> aux2;
  std::string alpha = a;
  if(mainGraph->select[0].first.compare("*")!=0){
    std::vector< std::pair<std::string,std::string> >::iterator it;
    for(it = mainGraph->select.begin(); it != mainGraph->select.end(); ++it) {
      std::string s = it->first;
      size_t found = s.find("(");
      if(found==std::string::npos){
        if((it->second).compare("")==0){
          aux.push_back(s);
        }else{
          next();
          std::cout << a << " = " << s <<"\n";
          aux.push_back(a);
        }
      }else{
        s.erase (0,found);
        s.pop_back();
        next();
        std::cout << a << " =std::map" << s << ")\n";
        std::string alpha2 = a;
        next();
        std::cout << a << " = krao(" << alpha2 << "," << alpha <<")\n";
        s = it->first;
        s.erase (found,s.size()-found);
        std::cout << it->second << " = " << s << "(" << a << ")\n";
        aux.push_back(it->second);
      }
    }
    std::cout << "return(" << aux[0];
    for(std::vector<std::string>::iterator it = aux.begin()+1; it != aux.end(); ++it) {
      std::cout << ", " << *it;
    }
    std::cout << ")\n";
  }else{
    std::cout << "return(" << a << ")\n";
  }
}
void driver::change_trees(driver::Ltree t2, int ind){
  for(int x = 0; x < trees.size(); x++ ){
    if(x == ind){
      trees[x]=t2;
    }
  }
}

void driver::copy_tree(driver::Ltree t){
  for(int x = 0; x<t.ltree.size(); x++){
    l->ltree.push_back(t.ltree[x]);
  }
}




void driver::print_joins(){
  for(int x = 0; x < mainGraph->join.size();x++){
    for(int y = 0; y < mainGraph->join[x].size();y++){
      std::cout <<  mainGraph->join[x].at(y) << " ,";
    }
    std::cout << "\n";
  }
}

    /*se o grafo for em circulo não funciona*/
std::string driver::giveMeRoot(std::string root){
    if(mainGraph->join.size() > 0){
        for(std::vector<std::vector<std::string> >::iterator it = mainGraph->join.begin(); it != mainGraph->join.end(); ++it) {
            if((it->at(2)).compare(root) == 0){
              //std::cout << it->at(1) << "\n";
              return giveMeRoot(it->at(1));
            }
          }
        }
        return root;
      }
//---------------------------------------
//              YACC
//---------------------------------------

void driver::final(){
    (*l) = trees[0];
    resolve(0);
    returnf();
}

void driver::select_asterisco(){
    mainGraph->add_select("*","");
}

void driver::select_term(int s1){
    mainGraph->add_select(types[s1].expr,"");
}

void driver::select_term_name(int s1, const std::string& s3){
  mainGraph->add_select(types[s1].expr,s3);
}

void driver::subfromlist_name(const std::string& s1){
  mainGraph->newRoot(s1);
}

void driver::subfromlist_join_literal_literal(int dolar6, int  dolar4){
    mainGraph->add_join(types[dolar6].type,getTable(types[dolar4].type),getTable(types[dolar6].type));
}

void driver::subfromlist_join_nameAS(const std::string& s4,const std::string& s2 ){
    mainGraph->tables[s4]=mainGraph->tables[s2];
}

void driver::subfromlist_join_as_literal_literal(const std::string& s2, const std::string& s4,int dolar6, int dolar8){
        int s6 = dolar6;
        int s8 = dolar8;
        mainGraph->add_join(types[s8].type,getTable(types[s6].type),getTable(types[s8].type));
        mainGraph->tables[s4]=mainGraph->tables[s2];
    }

void driver::expr_and(int  s1, int s3){
   change_trees(join_trees(trees[s1],trees[s3],"AND"),s1);
}

void driver::expr_or(int s1, int s3){
    change_trees(join_trees(trees[s1],trees[s3],"OR"),s1);
}

void driver::exp_term(int s1){
    trees[ itr++ ] =  create_tree(types[s1].expr,types[s1].type) ;
}


void driver::term_bbop_term(int s1, const std::string& s2, int s3){
      types[s1].expr.append(s2);
      types[s1].expr.append(types[s3].expr);
                if((types[s1].type.size()!=0)&&(types[s3].type.size()!=0)){
                      mainGraph->add_join(types[s3].type,getTable(types[s1].type),getTable(types[s3].type));
                      types[s1].type = "JOIN";
                } else {
                      mainGraph->add_map_filter(types[s1].type,types[s1].expr);
                      types[s1].type = "NOT JOIN";
                }
}

void driver::term_ibop_term(int s1, const std::string& s2, int s3){
    types[s1].expr.append(s2);
    types[s1].expr.append(types[s3].expr) ;
    types[s1].type.append(types[s3].type);
}

void driver::factor_name_arg(const std::string& s1,int s3){
      std::string s = "";
      s.append(s1);
      s.append("(");
      s.append(types[s3].expr);
      s.append(")");
      types[s3].expr = s;
}

void driver::factor_not_factor(const std::string& s1, int s2){
      std::string s = "";
      s.append(s1);
      s.append(" ");
      s.append(types[s2].expr);
      types[s2].expr = s;
}

void driver::args1_virgula(int s1, int s3){
    types[s1].expr.append( ",");
    types[s1].expr.append(types[s3].expr) ;
    types[s1].type.append(types[s3].type);
}

void driver::args1_espaco(int s1, int s3){
    types[s1].expr.append( " ");
    types[s1].expr.append(types[s3].expr) ;
    types[s1].type.append(types[s3].type);
}

void driver::groupbyListSub_literal(int dolar1 ){
   std::string Table = getTable(types[dolar1].expr);
   std::string s = Table;
   s.append(".");
   s.append(types[dolar1].expr);
   mainGraph->add_groupby(Table,s);
}


void driver::literal_name(const std::string& s1){
    std::string s = getTable(s1);
    s.append(".");
      if(s.compare(".")!=0){
          types[itr2].type = "";
          types[itr2++].expr = s1;
      }else{
          s.append(s1);
          Par novo;
          novo.type = s;
          novo.expr= s;
          types.push_back( novo);
          //types[itr2++].expr = s;
      }
}

void driver::literal_name_name(const std::string& s1, const std::string& s3){
      std::string s = "";
      s.append(s1);
      s.append(".");
      s.append(s3);
      types[itr2++].type = s;
      types[itr2++].expr = s;
}

void driver::literal_date(const std::string& s1){
      types[itr2].expr = s1 ;
      types[itr2++].type = "";
}

void driver::literal_constant(const std::string& s1){
      types[itr2].expr = s1 ;
      types[itr2++].type = "";
}

void driver::literal_bool(const std::string& s1){
      types[itr2].expr = s1 ;
      types[itr2++].type = "";
    }

void driver::literal_any(const std::string& s1){
      types[itr2].expr = s1 ;
      types[itr2++].type = "";
}

void driver::literal_all(const std::string& s1){
      types[itr2].expr = s1 ;
      types[itr2++].type = "";
}

//int main(){
//  mainGraph->add_select("lineitem.orderkey","");
//  mainGraph->add_select("sum(lineitem.extendedprice * (1 - lineitem.discount))","revenue");
//  mainGraph->add_select("orders.orderdate","");
//  mainGraph->add_select("lineitem.shippriority","");
//
//  mainGraph->add_table("lineitem","lineitem.shippriority","dimension");
//  mainGraph->add_table("orders","orders.orderdate","dimension");
//  mainGraph->add_table("customer","customer.mksegment","dimension");
//  mainGraph->add_table("lineitem","lineitem.shipdate","dimension");
//
//  mainGraph->newRoot("customer");
//  mainGraph->add_std::map_filter("customer.mksegment","mktsegment = 'BUILDING'");
//  trees.push_back(create_tree("mktsegment = 'BUILDING'","customer.mksegment"));
//  mainGraph->add_std::map_filter("orders.orderdate","orders.orderdate < date '1995-03-15'");
//  trees.push_back(create_tree("orders.orderdate < date '1995-03-15'","orders.orderdate"));
//  change_trees(join_trees(trees[0],trees[1],"AND"),0);
//  mainGraph->add_std::map_filter("lineitem.shipdate","lineitem.shipdate > date '1995-03-15'");
//  trees.push_back(create_tree("lineitem.shipdate > date '1995-03-15'","lineitem.shipdate"));
//  change_trees(join_trees(trees[0],trees[2],"AND"),0);
//  mainGraph->add_groupby("lineitem","lineitem.l_orderkey");
//  mainGraph->add_groupby("orders","orders.orderdate");
//  mainGraph->add_groupby("lineitem","lineitem.shippriority");
//  mainGraph->add_join("orders.o_custkey","customer","orders");
//  mainGraph->add_join("lineitem.l_orderkey","orders","lineitem");
//  copy_tree(trees[0]);
//  //print_tree();
//  resolve(0);
//  dot_if_needed();
//  returnf();
//  return 0;
//}

// std::string driver::Ltree::all_same_table_aux(int indice){
//   ltree.reserve(1+indice);
//   if(ltree[indice].compare("AND")==0 || ltree[indice].compare("OR")==0){
//     return all_same_table_aux(ind_left_child(indice));
//   } else {
//     return mainGraph->search_table_filter(ltree[indice]);
//   }
// }

// bool driver::Ltree::all_same_table(int indice){
//   ltree.reserve(1+indice);
//   std::string s = all_same_table_aux(indice);
//   std::vector<std::string> aux = childs(indice);
//   std::string s2;
//   for(std::vector<std::string>::iterator it = aux.begin(); it != aux.end(); ++it) {
//     if(it->compare("AND")!=0 && it->compare("OR")!=0 && it->compare("NULL")!=0){
//       s2 = mainGraph->search_table_filter(*it);
//       if(s2.compare(s) != 0){
//         return false;
//       }
//     }
//   }
//   return true;
// }

}  // namespace sql
