/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef LAQ_INTERFACE_SRC_LAQ_STATEMENT_HH_
#define LAQ_INTERFACE_SRC_LAQ_STATEMENT_HH_
#include <string>
#include <vector>
#include "laq-driver.hh"

namespace laq {

class driver::statement {
 public:
  statement(const std::string& lvar,
            const std::string& op,
            const std::vector<std::string>& rvars,
            const std::string& expr);

 private:
  std::string leftvar;
  std::string operation;
  std::vector<std::string> rightvars;
  std::string expression;
};  // class statement

}  // namespace laq

#endif  // LAQ_INTERFACE_SRC_LAQ_STATEMENT_HH_
