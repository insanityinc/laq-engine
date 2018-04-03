/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#include "src/laq_statement.hpp"

namespace laq {

driver::parsing_tree::statement::statement(
                                  const std::string& op,
                                  const std::string& expr,
                                  const std::vector<std::string>& rvars) {
  operation = op;
  expression = expr;
  rightvars = rvars;
}

std::string
driver::parsing_tree::statement::toString() {
  std::string res = operation + "(";
  if (!expression.empty()) {
    res += expression + ")";
  } else if (!rightvars.empty()) {
    int i = -1;
    for (auto& rv : rightvars) {
      if (++i)
        res += ",";
      res += rv;
    }
    res+=")";
  }
  return res;
}

}  // namespace laq
