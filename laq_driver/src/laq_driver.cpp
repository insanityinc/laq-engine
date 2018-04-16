/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#include <string>
#include <tuple>
#include <vector>
#include "include/database.hpp"
#include "include/laq_driver.hpp"
#include "src/parsing_tree.hpp"

namespace laq {

driver::driver() : trace_scanning(false), trace_parsing(false) {
  laquery = new ParsingTree();
}

driver::~driver() {
  // delete laquery;
}

int
driver::parse(const std::string &f) {
  file = f;
  if (f.length() <= 4) {
    error("Error: Filename too short");
    return 1;
  }
  if (f.substr(f.length() - 4) != ".laq") {
    error("Error: LAQ file expected");
    return 2;
  }
  scan_begin();
  yy::laq_parser parser(this);
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

void
driver::addDatabaseVar(const std::string& table,
                       const std::string& attribute) {
  dbvars.insert(std::make_pair(table, attribute));
}

int
driver::insertStatement(const std::string& lvar,
                        const std::string& op,
                        const std::vector<std::string>& rvars,
                        const std::string& expr) {
  if (rvars.empty() && expr.empty())
    return 1;
  laquery->insertStatement(lvar, op, rvars, expr);
  return 0;
}

std::string
driver::getQuery() {
  return laquery->getQuery();
}

std::string
driver::toCpp(engine::Database db) {
  return laquery->toCpp(db);
}

size_t
driver::count_exp_vars() {
  return expvars.size();
}

void
driver::add_exp_var(const std::string& var) {
  if (!(std::find(expvars.begin(), expvars.end(), var) != expvars.end())) {
    expvars.push_back(var);
  }
}

std::vector<std::string>
driver::clear_exp_vars() {
  std::vector<std::string> tmp = expvars;
  expvars.clear();
  return tmp;
}


}  // namespace laq
