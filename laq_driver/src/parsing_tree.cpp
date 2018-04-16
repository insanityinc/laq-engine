/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#include <string>
#include <tuple>
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
         "#include \"include/types.hpp\"\n"
         "\n";
  return out;
}

MatrixTypeMap getMatrixTypeMap() {
  MatrixTypeMap matTypeMap = {
    {std::make_tuple("filter", "Bitmap", ""), "FilteredBitVector"},
    {std::make_tuple("filter", "DecimalVector", ""), "FilteredBitVector"},

    {std::make_tuple("hadamard", "FilteredBitVector", "FilteredBitVector"),
      "FilteredBitVector"},
    {std::make_tuple("hadamard", "FilteredBitVector", "DecimalVector"),
      "FilteredDecimalVector"},

    {std::make_tuple("lift", "DecimalVector", ""), "DecimalVector"},
    {std::make_tuple("lift", "DecimalVector", "DecimalVector"),
      "DecimalVector"},
    {std::make_tuple("sum", "FilteredDecimalVector", ""), "Decimal"}
  };

  return matTypeMap;
}

AttributeTypes getAttributeTypes(engine::Database& db) {
  AttributeTypes attrTypes;

  for (const auto& tbl : db.tables) {
    for (const auto& attr : tbl.second) {
      if (db.isDimension(tbl.first, attr.first)) {
        attrTypes[tbl.first + "." + attr.first] = "Bitmap";
      } else if (db.isMeasure(tbl.first, attr.first)) {
        attrTypes[tbl.first + "." + attr.first] = "DecimalVector";
      }
    }
  }
  return attrTypes;
}

std::string makeFilterPredicate(std::string type,
                                std::string var,
                                std::string exp) {
  std::string out;
  out += "inline bool filter_" + var + "(std::vector<engine::";
  if (type == "Bitmap") {
    out += "Literal";
  } else {
    out += "Decimal";
  }
  out += "> args){\n";
  out += "  return " + exp + ";\n";
  out += "}\n";

  return out;
}

std::string makeLiftPredicate(std::string var,
                              std::string exp) {
  std::string out;
  out += "inline engine::Decimal lift_" + var +
         "(std::vector<engine::Decimal> args) {\n";
  out += "  return " + exp + ";\n";
  out += "}\n";

  return out;
}

std::string
driver::ParsingTree::predicates(AttributeTypes& attrTypes,
                                const std::string& var) {
  std::string out;

  if (attrTypes.find(var) == attrTypes.end()) {
    auto& rvars = tree.at(var).rightvars;
    auto& op = tree.at(var).operation;
    auto& exp = tree.at(var).expression;

    for (const auto& attr : rvars) {
      out += predicates(attrTypes, attr);
    }

    if (op == "filter") {
      return out + makeFilterPredicate(attrTypes.at(rvars.at(0)), var, exp);
    } else if (op == "lift") {
      return out + makeLiftPredicate(var, exp);
    }
  }
  return out;
}

std::string getTmpDeclaration(const std::string& type,
                              const std::string& var,
                              std::string rightvar) {
  std::string out;

  auto dot = rightvar.find(".");
  if (dot != std::string::npos) {
    rightvar = rightvar.replace(dot, 1, "_");
  }

  out += "  engine::" + type + " *" + var + " =\n";
  out += "    new engine::" + type + "(";
  if (type != "Decimal") {
    out += rightvar + "->nBlocks";
  }
  out += ");\n";

  return out;
}

std::string getDimensionFilterDeclaration(const std::string& type,
                                          const std::string& var,
                                          std::string rightvar) {
  std::string out;
  auto dot = rightvar.find(".");
  if (dot != std::string::npos) {
    rightvar = rightvar.replace(dot, 1, "_");
  }
  out += "  engine::" + type + " *" + var + "_pred =\n";
  out += "    new engine::" + type + "(" + rightvar + "->nLabelBlocks);\n";
  out += "  engine::" + type + " *" + var + " =\n";
  out += "    new engine::" + type + "(" + rightvar + "->nBlocks);\n";
return out;
}

std::pair<std::string, std::string>
driver::ParsingTree::declareTmpVars(MatrixTypeMap& matTypeMap,
                                    AttributeTypes& attrTypes,
                                    const std::string& var) {
  if (var == "$$") {
    std::string cpp;
    for (const auto& attr : tree.at(var).rightvars) {
      cpp += driver::ParsingTree::declareTmpVars(
        matTypeMap, attrTypes, attr).second;
    }
    return std::make_pair("", cpp);
  }

  if (attrTypes.find(var) != attrTypes.end()) {
    std::string tp = attrTypes.at(var);
    return std::make_pair(tp, "");
  }

  std::string op = tree.at(var).operation;
  std::vector<std::pair<std::string, std::string>> calls;

  std::vector<std::string>& v = tree.at(var).rightvars;

  engine::Size i = 0;
  for (i = 0; i < v.size(); ++i) {
    calls.push_back(
      driver::ParsingTree::declareTmpVars(matTypeMap, attrTypes, v[i])
    );
  }
  for (; i < 2; ++i) {
    calls.push_back(std::make_pair("",""));
  }

  std::string type = matTypeMap.at(
    std::make_tuple(op, calls.at(0).first, calls.at(1).first)
  );
  // store type somewhere

  std::string cpp;
  for (i = 0; i < v.size(); ++i) {
    cpp += calls.at(i).second;
  }

  if (op == "filter" && calls.at(0).first == "Bitmap") {
    cpp += getDimensionFilterDeclaration(type, var, v[0]);
  } else {
    cpp += getTmpDeclaration(type, var, v[0]);
  }
  return std::make_pair(type, cpp);
}

std::string getAttrDeclaration(std::string type, std::string var) {
  std::string out;
  auto dot = var.find(".");

  out += "  engine::" + type + " *" + var.replace(dot, 1, "_") + " =\n";
  out += "    new engine::" + type + "(db.data_path,\n";
  out += "      db.database_name,\n";
  out += "      \"" + var.substr(0, dot) + "\",\n";
  out += "      \"" + var.substr(dot+1, var.size()) + "\");\n";

  return out;
}

std::string
declareAttrVars(AttributeTypes& attrTypes) {
  std::string out;
  for (const auto& attr : attrTypes) {
    out += getAttrDeclaration(attr.second, attr.first);
  }
  return out;
}



std::string
driver::ParsingTree::toCpp(engine::Database& db) {
  std::string out;
  // -) Add Header comments
  out += copyright();
  // 1) Add the needed header files:
  out += header();

  // 2) Define the necessary predicates
  AttributeTypes attrTypes = getAttributeTypes(db);
  out += predicates(attrTypes, "$$") + "\n";

  // -) Open main function
  out += "int main() {\n";

  // 3) Select the database
  out += "  engine::Database db(\n"
         "    \"" + db.data_path + "\",\n"
         "    \"" + db.database_name + "\",\n"
         "    false);\n\n";

  // 4) Load attributes metadata
  out += declareAttrVars(attrTypes) + "\n";

  // 5) Declare temporary matrices
  MatrixTypeMap matTypeMap = getMatrixTypeMap();

  std::pair<std::string, std::string> tmpVars =
    declareTmpVars(matTypeMap, attrTypes, "$$");

  out += tmpVars.second + "\n";

  // 6) Build the for loops



  // -) Close main function
  out += "  return 0;\n"
         "}\n";

  return out;
}

}  // namespace laq
