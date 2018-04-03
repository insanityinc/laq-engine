/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef LAQ_DRIVER_SRC_PARSING_TREE_H_
#define LAQ_DRIVER_SRC_PARSING_TREE_H_
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "include/laq_driver.hpp"

namespace laq {

class driver::parsing_tree {
 public:
  parsing_tree();

  void insert_statement(const std::string& lvar,
                        const std::string& op,
                        const std::vector<std::string>& rvars,
                        const std::string& expr);

  std::string getQuery();

 private:
  class statement;

  std::map<std::string, statement> tree;
  std::vector<std::string> vars;
};  // class parsing_tree

}  // namespace laq

#endif  // LAQ_DRIVER_SRC_PARSING_TREE_H_
