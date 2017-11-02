/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef LAQ_INTERFACE_SRC_LAQ_STATEMENT_HH_
#define LAQ_INTERFACE_SRC_LAQ_STATEMENT_HH_
#include <string>
#include <vector>
#include "parsing-tree.hh"

namespace laq {

class driver::parsing_tree::statement {
 public:
  statement(const std::string& op,
            const std::string& expr,
            const std::vector<std::string>& rvars);

  std::string operation;
  std::string expression;
  std::vector<std::string> rightvars;
};  // class statement

}  // namespace laq

#endif  // LAQ_INTERFACE_SRC_LAQ_STATEMENT_HH_
