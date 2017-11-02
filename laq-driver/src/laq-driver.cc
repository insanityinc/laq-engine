/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "laq-driver.hh"
#include "laq-parser.hh"
#include "parsing-tree.hh"

namespace laq {

driver::driver() : trace_scanning(false), trace_parsing(false) {
  laquery = new parsing_tree();
}

driver::~driver() {
  // delete laquery;
}

int
driver::parse(const std::string &f) {
  file = f;
  if(f.length() <= 4 ) {
    error("Error: Filename too short");
    return 1;
  }
  if(f.substr(f.length() - 4) != ".laq") {
    error("Error: LAQ file expected");
    return 2;
  }
  scan_begin();
  yy::laq_parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end();
  return res;
}

void
driver::error(const yy::location& l, const std::string& m) {
  std::cerr << l << ": " << m << std::endl;
}

void
driver::error(const std::string& m) {
  std::cerr << m << std::endl;
}

void
driver::add_var(const std::string& var) {
  variables.insert(var);
}

bool
driver::var_exists(const std::string& var) {
  return (variables.find(var) != variables.end());
}

int
driver::insert_statement(const std::string& lvar,
                         const std::string& op,
                         const std::vector<std::string>& rvars,
                         const std::string& expr) {
  if (rvars.empty() && expr.empty())
    return 1;
  laquery->insert_statement(lvar, op, rvars, expr);
  return 0;
}

void
driver::add_exp_var(const std::string& var) {
  expvars.push_back(var);
}

std::vector<std::string>
driver::clear_exp_vars() {
  std::vector<std::string> tmp = expvars;
  expvars.clear();
  return tmp;
}


}  // namespace laq
