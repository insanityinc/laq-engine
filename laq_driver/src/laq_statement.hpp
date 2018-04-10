/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef LAQ_DRIVER_SRC_LAQ_STATEMENT_H_
#define LAQ_DRIVER_SRC_LAQ_STATEMENT_H_
#include <string>
#include <vector>
#include "src/parsing_tree.hpp"

namespace laq {

class driver::ParsingTree::Statement {
 public:
  Statement(const std::string& op,
            const std::string& expr,
            const std::vector<std::string>& rvars);

  std::string toString();

  std::string operation;
  std::string expression;
  std::vector<std::string> rightvars;
};  // class statement

}  // namespace laq

#endif  // LAQ_DRIVER_SRC_LAQ_STATEMENT_H_
