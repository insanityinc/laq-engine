/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef LAQ_INTERFACE_INCLUDE_LAQ_DRIVER_HH_
#define LAQ_INTERFACE_INCLUDE_LAQ_DRIVER_HH_
#include <set>
#include <string>
#include <vector>
#include "laq-parser.hh"

// Tell Flex the lexer's prototype ...
#define YY_DECL                                 \
  yy::laq_parser::token_type                    \
  yylex(yy::laq_parser::semantic_type* yylval,  \
        yy::laq_parser::location_type* yylloc,  \
        laq::driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;

namespace laq {

// Conducting the whole scanning and parsing
// of LAQ (Linear Algebra Query language)
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

  // Error handling.
  void error(const yy::location& l, const std::string& m);
  void error(const std::string& m);

  // Check variable definition
  void addvar(const std::string& var);
  bool varexists(const std::string& var);

  // Adds a new statement to the list
  void insertStatement();

 private:
  // Staments of a LA script
  class statement {
   public:
    statement();

   private:
    std::string lvar;
    std::string operation;
    std::vector<std::string> rvars;
    std::string expression;
  };  // class statement

  // Assigned variables of a LA script
  std::set<std::string> variables;

  // List of statements
  std::vector<statement> laquery;

  // Tree of statements
  // util::
};  // class driver
}  // namespace laq

#endif  // LAQ_INTERFACE_INCLUDE_LAQ_DRIVER_HH_
