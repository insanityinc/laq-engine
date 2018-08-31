/*
 * Copyright (c) 2018 João Afonso, Luís Albuquerque, and Rafael Fernandes.
 * All rights reserved.
 */
#include "src/ltree.hpp"

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

} // namespace sql
