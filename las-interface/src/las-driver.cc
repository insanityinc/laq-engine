/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "las-driver.hh"
#include "las-parser.hh"

namespace las {

driver::driver() : trace_scanning(false), trace_parsing(false) {
}

driver::~driver() {
}

int
driver::parse(const std::string &f) {
  file = f;
  scan_begin();
  yy::las_parser parser(*this);
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

}  // namespace las
