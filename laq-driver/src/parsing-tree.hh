/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef LAQ_INTERFACE_SRC_PARSING_TREE_HH_
#define LAQ_INTERFACE_SRC_PARSING_TREE_HH_
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "laq-driver.hh"

namespace laq {

class driver::parsing_tree {
 public:
  parsing_tree();

  void insert_statement(const std::string& lvar,
                        const std::string& op,
                        const std::vector<std::string>& rvars,
                        const std::string& expr);

 private:
  class statement;

  std::map<std::string, statement> tree;
};  // class parsing_tree

}  // namespace laq

#endif  // LAQ_INTERFACE_SRC_PARSING_TREE_HH_
