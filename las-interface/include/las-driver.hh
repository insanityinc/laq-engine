/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef LAS_INTERFACE_INCLUDE_LAS_DRIVER_HH_
#define LAS_INTERFACE_INCLUDE_LAS_DRIVER_HH_
#include <set>
#include <string>
#include <vector>
#include "las-parser.hh"

// Tell Flex the lexer's prototype ...
#define YY_DECL                                 \
  yy::las_parser::token_type                    \
  yylex(yy::las_parser::semantic_type* yylval,  \
        yy::las_parser::location_type* yylloc,  \
        las::driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;

namespace las {

// Conducting the whole scanning and parsing
// of LAS (Linear Algebra Scripting language)
class driver {
 public:
  driver();
  virtual ~driver();

  // Handling the scanner.
  void scan_begin();
  void scan_end();
  bool trace_scanning;

  // Run the parser. Return 0 on success.
  int parse(const std::string& f);
  std::string file;
  bool trace_parsing;

  // Check variable definition
  void add_variable();
  void variable_exists();

  //
  void insertStatement();

  // Error handling.
  void error(const yy::location& l, const std::string& m);
  void error(const std::string& m);

 private:
  // Staments of a LA script
  class statement {
    statement();
  };  // class statement

  // Assigned variables of a LA script
  std::set<std::string> variables;

  // List of statements
  std::vector<statement> lascript;

  // Tree of statements
  // util::
};  // class driver
}  // namespace las

#endif  // LAS_INTERFACE_INCLUDE_LAS_DRIVER_HH_
