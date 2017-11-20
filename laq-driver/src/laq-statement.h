/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef LAQ_DRIVER_SRC_LAQ_STATEMENT_H_
#define LAQ_DRIVER_SRC_LAQ_STATEMENT_H_
#include <string>
#include <vector>
#include "src/parsing-tree.h"

namespace laq {

class driver::parsing_tree::statement {
 public:
  statement(const std::string& op,
            const std::string& expr,
            const std::vector<std::string>& rvars);

  std::string toString();

  std::string operation;
  std::string expression;
  std::vector<std::string> rightvars;
};  // class statement

}  // namespace laq

#endif  // LAQ_DRIVER_SRC_LAQ_STATEMENT_H_
