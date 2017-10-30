/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "laq-statement.hh"

namespace laq {

  driver::statement::statement(const std::string& lvar,
                               const std::string& op,
                               const std::vector<std::string>& rvars,
                               const std::string& expr) {
    leftvar = lvar;
    operation = op;
    rightvars = rvars;
    expression = expr;
}

}  // namespace laq
