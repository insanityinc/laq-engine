/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#include <string>
#include <vector>
#include <utility>
#include "include/database.hpp"
#include "include/laq_driver.hpp"
#include "src/laq_statement.hpp"
#include "src/parsing_tree.hpp"

namespace laq {

driver::ParsingTree::ParsingTree() {
}


void
driver::ParsingTree::insertStatement(const std::string& lvar,
                                      const std::string& op,
                                      const std::vector<std::string>& rvars,
                                      const std::string& expr) {
  Statement stt(op, expr, rvars);
  tree.insert(std::pair<std::string, Statement>(lvar, stt));
  vars.push_back(lvar);
}

std::string
driver::ParsingTree::getQuery() {
  std::string res = "";
  for (const auto& v : vars) {
    if (v != "$$")
      res += v + "=";
    res += tree.at(v).toString() + "\n";
  }
  return res;
}

std::string copyright() {
  std::string out;
  out += "/*\n"
         " * Copyright (c) 2018 João Afonso. All rights reserved.\n"
         " *\n"
         " * This query was automatically generated\n"
         " */\n";
  return out;
}

std::string header() {
  std::string out;
  out += "#include <string>\n"
         "#include <vector>\n"
         "#include \"include/block.hpp\"\n"
         "#include \"include/database.hpp\"\n"
         "#include \"include/dot.hpp\"\n"
         "#include \"include/filter.hpp\"\n"
         "#include \"include/fold.hpp\"\n"
         "#include \"include/krao.hpp\"\n"
         "#include \"include/lift.hpp\"\n"
         "#include \"include/matrix.hpp\"\n"
         "#include \"include/types.hpp\"\n";
  return out;
}

MatrixTypeMap getMatrixTypeMap() {
  // MatrixTypeMap matTypeMap = {
  //   {"filter", {{"Bitmap", {{"", "FilteredBitVector"}}},
  //               {"DecimalVector", {{"", "FilteredBitVector"}}}
  //   }},
  //   {"hadamard",
  //     {{"FilteredBitVector", {{"FilteredBitVector", "FilteredBitVector"}}},
  //      {"FilteredBitVector", {{"DecimalVector", "FilteredDecimalVector"}}}
  //   }},
  //   {"lift", {{"DecimalVector", {{"", "DecimalVector"}}}
  //   }},
  //   {"sum", {{"FilteredDecimalVector", {{"", "Decimal"}}}
  //   }},
  // };

  MatrixTypeMap matTypeMap = {
    {std::make_tuple("filter", "Bitmap", ""), "FilteredBitVector"},
    {std::make_tuple("filter", "DecimalVector", ""), "FilteredBitVector"},

    {std::make_tuple("hadamard", "FilteredBitVector", "FilteredBitVector"),
      "FilteredBitVector"},
    {std::make_tuple("hadamard", "FilteredBitVector", "DecimalVector"),
      "FilteredDecimalVector"},

    {std::make_tuple("lift", "DecimalVector", ""), "DecimalVector"},
    {std::make_tuple("sum", "FilteredDecimalVector", ""), "Decimal"}
  };

  return matTypeMap;
}

std::string
driver::ParsingTree::getType(MatrixTypeMap& matTypeMap,
                             AttributeTypes& attrTypes,
                             const std::string& var) {
  if (attrTypes.find(var) != attrTypes.end()) {
    return attrTypes[var];
  }

  std::string op = tree.at(var).operation;
  std::string fst, snd;

  std::vector<std::string>& v = tree.at(var).rightvars;

  if (v.size() > 0) {
    fst = driver::ParsingTree::getType(matTypeMap, attrTypes, v[0]);
    if (v.size() > 1) {
      snd = driver::ParsingTree::getType(matTypeMap, attrTypes, v[1]);
    } else {
      snd = "";
    }
  } else {
    fst = "";
    snd = "";
  }

  std::string st = matTypeMap.at(std::make_tuple(op, fst, snd));
  attrTypes[var] = st;

  return attrTypes[var];
}

std::string
driver::ParsingTree::toCpp(engine::Database db) {
  std::string out;
  // -) Add Header comments
  out += copyright();
  // 1) Add the needed header files:
  out += header();

  // 2) Define the necessary predicates


  // -) Open main function
  out += "int main() {\n";

  // 3) Select the database
  out += "  engine::Database db(\n"
         "    \"" + db.data_path + "\",\n"
         "    \"" + db.database_name + "\");\n\n";

  // 4) Load attributes metadata
    // Get the leafs of the tree
    // Add them to the string

  // 5) Declare temporary matrices
  MatrixTypeMap matTypeMap = getMatrixTypeMap();
  AttributeTypes attrTypes;// = getAttributeTypes();
  for (const auto& t : tree) {
    std::string v = t.first;
    // out += declare(v, getType(matTypeMap, attrTypes, v));
    std::string type = getType(matTypeMap, attrTypes, v);
  }


  if(matTypeMap.empty()) {}

  // 6) Build the for loops

  // -) Close main function
  out += "  return 0;\n"
         "}\n";
  return out;
}

}  // namespace laq
