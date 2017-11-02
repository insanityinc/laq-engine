/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "laq-statement.hh"
#include "parsing-tree.hh"

namespace laq {

driver::parsing_tree::parsing_tree() {
}


void
driver::parsing_tree::insert_statement(const std::string& lvar,
                                       const std::string& op,
                                       const std::vector<std::string>& rvars,
                                       const std::string& expr) {
  statement stt(op, expr, rvars);
  tree.insert(std::pair<std::string, statement>(lvar, stt));
}

}  // namespace laq
