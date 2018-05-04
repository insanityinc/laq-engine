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
  out += "#include <chrono>\n"
         "#include <iostream>\n"
         "#include <string>\n"
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
    {std::make_tuple("", "", ""), ""},

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

std::string normalizeVar(std::string var) {
  auto dot = var.find(".");
  if (dot != std::string::npos) {
    return var.replace(dot, 1, "_");
  }
  return var;
}

bool isFold(std::string op) {
  return op == "count"
      || op == "sum"
      || op == "avg"
      || op == "min"
      || op == "max";
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
                                std::string exp)
{
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
                              std::string exp)
{
  std::string out;
  out += "inline engine::Decimal lift_" + var +
         "(std::vector<engine::Decimal> args) {\n";
  out += "  return " + exp + ";\n";
  out += "}\n";

  return out;
}

std::string
driver::ParsingTree::predicates(AttributeTypes& attrTypes,
                                const std::string& var)
{
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
                              std::string rightvar)
{
  std::string out;

  out += "  engine::" + type + " *" + normalizeVar(var) + " =\n";
  out += "    new engine::" + type + "(";
  if (type != "Decimal") {
    out += normalizeVar(rightvar) + "->nBlocks";
  }
  out += ");\n";

  return out;
}

std::string getDimensionFilterDeclaration(const std::string& type,
                                          const std::string& var,
                                          std::string rightvar)
{
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
                                    const std::string& var)
{
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
    if (isFold(op)) {
      if (type != "Decimal") {
        cpp += "  engine::" + type + "Acc *" + normalizeVar(var) + "_acc =\n";
        cpp += "    new engine::" + type + "Acc();\n";
      }
    }
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


void mergeAttributeMaps(AttributeAlloc& base, const AttributeAlloc& tmp) {
  for (const auto& child_attr : tmp) {
    if (base.find(child_attr.first) != base.end()) {
      std::get<1>(base.at(child_attr.first)) += std::get<1>(child_attr.second);
    } else {
      base[child_attr.first] = child_attr.second;
    }
  }
}

AttributeAlloc
driver::ParsingTree::getAttrAlloc(MatrixTypeMap& matTypeMap,
                                  AttributeTypes& attrTypes,
                                  const std::string& var)
{
  AttributeAlloc out;

  if (attrTypes.find(var) != attrTypes.end()) {
    std::string type = attrTypes.at(var);
    out[var] = std::make_tuple(type, 1, 0);
    return out;
  }

  if (var != "$$") {
    // get current type based on Mat Types map
    std::string op = tree.at(var).operation;
    std::vector<std::string> calls;

    for (const auto& attr : tree.at(var).rightvars) {
      AttributeAlloc tmp = driver::ParsingTree::getAttrAlloc(
        matTypeMap, attrTypes, attr);
      mergeAttributeMaps(out, tmp);
      calls.push_back( std::get<0>(tmp.at(attr)) );
    }
    for (engine::Size i = tree.at(var).rightvars.size(); i < 2; ++i) {
      calls.push_back("");
    }

    std::string type = matTypeMap.at(
      std::make_tuple(op, calls.at(0), calls.at(1))
    );

    out[var] = std::make_tuple(type, 1, 0);
  } else {

    for (const auto& attr : tree.at(var).rightvars) {
      AttributeAlloc tmp = driver::ParsingTree::getAttrAlloc(
        matTypeMap, attrTypes, attr);
      mergeAttributeMaps(out, tmp);
    }
  }

  return out;
}

std::vector<driver::ParsingTree::ForLoop>
driver::ParsingTree::getForLoops(AttributeTypes& attrTypes,
                                 AttributeAlloc& attrAlloc,
                                 const std::string& var) {
  std::vector<driver::ParsingTree::ForLoop> out;

  if (attrTypes.find(var) != attrTypes.end()) {
    std::string head = "  for(engine::Size i = 0; i < " + normalizeVar(var) +
                       "->nBlocks; ++i) {\n";
    std::string tail;
    if (std::get<2>(attrAlloc[var]) == 0) {
        tail += "    " + normalizeVar(var) + "->loadBlock(i);\n";
    }
    driver::ParsingTree::ForLoop db_attr(head, tail);
    out.push_back(db_attr);
  } else {
    // get child loops
    std::vector<std::vector<driver::ParsingTree::ForLoop>> childs;
    std::vector<std::string> rvars = tree.at(var).rightvars;

    for (const auto& attr : rvars) {
      auto child_res = getForLoops(attrTypes, attrAlloc, attr);
      childs.push_back(child_res);
      // add var usages
      std::get<2>(attrAlloc.at(attr)) ++;
    }
    // merge init loops
    for (const auto& res : childs) {
      for(engine::Size i = 0; i < res.size() - 1; ++i) {
        out.push_back( res.at(i) );
      }
    }

    if (var == "$$") {
      // delete vars in childs.deleteVars
      {
        driver::ParsingTree::ForLoop tmp (childs.at(0).back().head, "");

        for (const auto& res : childs) {
          tmp.tail += res.back().tail;
          for (const auto& delVar : res.back().deleteVars) {
            tmp.deleteVars.push_back(delVar);
          }
        }

        if (tmp.tail == "") {
          tmp.head = "";
        } else {
            tmp.tail += "  }";
        }

        for (const auto& attr : rvars) {
            tmp.tail += "\n  std::cout << (*" + normalizeVar(attr) +
                        ") << std::endl;";
        }

        for(const auto& delVar : tmp.deleteVars) {
          tmp.tail += "\n\n  delete " + delVar + ";";
        }

        tmp.tail += "\n\n";

        // print result
        // delete all rvars

        out.push_back(tmp);
      }
    } else {
      // get operation
      std::string op = tree.at(var).operation;
      std::string type = std::get<0>(attrAlloc.at(var));

      // if operation is dot
      if (op == "dot") {
        // close left loop
        {
          driver::ParsingTree::ForLoop& left_child = childs.at(0).back();
          left_child.tail += "  }\n";

          for(const auto& delVar : left_child.deleteVars) {
            left_child.tail += "\n  delete " + delVar + ";";
          }
          left_child.tail += "\n\n";

          out.push_back(left_child);
        }
        // add left to right delete vars
        driver::ParsingTree::ForLoop& rightChild = childs.at(1).back();
        rightChild.deleteVars.push_back(rvars[0]);

        // add dot to right loop
        //   -> alloc space for result
        rightChild.tail += "    " + normalizeVar(var) +
                           "->blocks[i] = new engine::" + type + "Block();\n";
        //   -> do dot product
        rightChild.tail += "    dot(*" + normalizeVar(rvars[0]) + ", *(" +
                           normalizeVar(rvars[1]) + "->blocks[i]), " +
                           normalizeVar(var) + "->blocks[i]);";

        // add delete right block to right loop
        auto rightInfo = attrAlloc.at(rvars[1]);

        if (std::get<1>(rightInfo) == std::get<2>(rightInfo)) {
          rightChild.tail += "\n    " + normalizeVar(rvars[1]) +
                             "->deleteBlock(i);";
        }
        rightChild.tail += "\n\n";

        out.push_back(rightChild);
      } else if (op == "filter" &&
                 std::get<0>(attrAlloc[rvars[0]]) == "Bitmap") {

        driver::ParsingTree::ForLoop loopLabels("", "");
        driver::ParsingTree::ForLoop loopDot(childs.at(0).back().head,
                                             childs.at(0).back().tail);

        // Filter attribute labels
        loopLabels.head += "  for (engine::Size i = 0; i < " +
                           normalizeVar(rvars[0]) +
                           "->nLabelBlocks; ++i) {\n";

        loopLabels.tail += "    " + normalizeVar(rvars[0]) +
                           "->loadLabelBlock(i);\n";

        loopLabels.tail += "    " + normalizeVar(var) + "_pred->blocks[i] = " +
                           "new engine::" + type + "Block();\n";

        loopLabels.tail += "    filter(filter_" + normalizeVar(var) + ",\n" +
                           "      {\n" +
                           "        *(" + normalizeVar(rvars[0]) +
                           "->labels[i])\n" +
                           "      },\n" +
                           "      " + normalizeVar(var) +
                           "_pred->blocks[i]);\n";

        loopLabels.tail += "    " + normalizeVar(rvars[0]) +
                           "->deleteLabelBlock(i);\n  }\n\n";

        out.push_back(loopLabels);

        // Dot product of the predicate with the bitmap
        loopDot.tail += "    " + normalizeVar(var) + "->blocks[i] = " +
                        "new engine::" + type + "Block();\n";

        loopDot.tail += "    dot(*" + normalizeVar(var) + "_pred, *(" +
                        normalizeVar(rvars[0]) + "->blocks[i]), " +
                        normalizeVar(var) + "->blocks[i]);\n";

        loopDot.tail += "    " + normalizeVar(rvars[0]) +
                        "->deleteBlock(i);\n\n";

        // Add the predicate to the deleteVars
        loopDot.deleteVars.push_back(normalizeVar(var) + "_pred");

        out.push_back(loopDot);

      } else if (isFold(op)) {
        driver::ParsingTree::ForLoop first(childs.at(0).back().head,
                                           childs.at(0).back().tail);

        driver::ParsingTree::ForLoop last("", "");

        // do fold function
        first.tail += "    " + op + "(*" + normalizeVar(rvars[0]) +
                      "->blocks[i], " + normalizeVar(var);
        if (type != "Decimal") {
          first.tail += "_acc";
        }
        first.tail += ");\n";

        // delete blocks
        for (const auto& rvar : rvars) {
          auto varInfo = attrAlloc.at(rvar);
          if (std::get<1>(varInfo) == std::get<2>(varInfo)) {
            first.tail += "    " + normalizeVar(rvar) + "->deleteBlock(i);\n";
          }
        }

        // close loop
        first.tail += "  }\n";

        if (type != "Decimal") {
          // acc to matrix
          first.tail += "\n  " + normalizeVar(var) + " = " +
                        normalizeVar(var) + "_acc.getMatrix();\n";
          // delete acc
          first.tail += "\n  delete " + normalizeVar(var) + "_acc;\n";
        }

        // delete vars
        for(const auto& delVar : childs.at(0).back().deleteVars) {
          first.tail += "\n  delete " + delVar + ";";
        }
        first.tail += "\n";

        out.push_back(first);

        last.head += "\n  for (engine::Size i = 0; i < " +
                     normalizeVar(var) + "->nBlocks; ++i) {\n";

        last.deleteVars.push_back(var);

        out.push_back(last);

      } else {
        // merge child loops
        // REPLICATED? CREATE FUNCTION TO DO THIS???
        {
          driver::ParsingTree::ForLoop tmp (childs.at(0).back().head, "");

          for (const auto& res : childs) {
            tmp.tail += res.back().tail;
            for (const auto& delVar : res.back().deleteVars) {
              tmp.deleteVars.push_back(delVar);
            }
          }

          // allocate block to store the operation result
          tmp.tail += "    " + normalizeVar(var) + "->blocks[i] = new engine::" +
                      type + "Block();\n";

          // do the needed operation
          if (op == "filter" || op == "lift") {
            tmp.tail += "    " + op + "(" + op +"_" + normalizeVar(var) + ",\n      {";

            bool first = true;
            for (const auto& rv : rvars) {
              if (!first) {
                tmp.tail += ",";
              }
              first = false;
              tmp.tail += "\n        *(" + normalizeVar(rv) + "->blocks[i])";
            }

            tmp.tail += "\n      },\n";
            tmp.tail += "      " + normalizeVar(var) + "->blocks[i]);";
          } else { //if (op == "krao" || op == "hadamard")

            tmp.tail += "    krao(*(" + normalizeVar(rvars[0]) +
                        "->blocks[i]), *(" + normalizeVar(rvars.size()>1 ? rvars[1] :rvars[0]) +
                        "->blocks[i]), " + normalizeVar(var) + "->blocks[i]);";
          }

          // add delete right block to right loop
          for (const auto& rvar : rvars) {
            auto varInfo = attrAlloc.at(rvar);
            if (std::get<1>(varInfo) == std::get<2>(varInfo)) {
              tmp.tail += "\n    " + normalizeVar(rvar) + "->deleteBlock(i);";
            }
          }
          tmp.tail += "\n\n";

          out.push_back(tmp);
        }
        // merge delete vars and keep them in "out"
      }
    }
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
  out += "  auto start = std::chrono::high_resolution_clock::now();\n\n";

  // 3) Select the database
  out += "  engine::Database db(\n"
         "    \"" + db.data_path + "\",\n"
         "    \"" + db.database_name + "\",\n"
         "    false);\n\n";

  // 4) Load attributes metadata
  out += declareAttrVars(attrTypes) + "\n";

  // 5) Declare temporary matrices
  MatrixTypeMap matTypeMap = getMatrixTypeMap();
  AttributeAlloc attrAlloc = getAttrAlloc(matTypeMap, attrTypes, "$$");

  // TODO (João) modify declareTmpVars to use attrAlloc
  std::pair<std::string, std::string> tmpVars =
    declareTmpVars(matTypeMap, attrTypes, "$$");

  out += tmpVars.second + "\n";

  // 6) Build the for loops
  std::vector<driver::ParsingTree::ForLoop> loops =
    getForLoops(attrTypes, attrAlloc, "$$");

  for(const auto& loop : loops) {
    out += loop.head + loop.tail;
  }

  // -) Close main function

  out += "  auto end = std::chrono::high_resolution_clock::now();\n"
         "  std::cout\n"
         "    << \"Completed in \"\n"
         "    << std::chrono::duration_cast<std::chrono::nanoseconds>"
         "(end-start).count()\n"
         "    << \" ns\"\n"
         "    << std::endl;\n\n"
         "  return 0;\n"
         "}\n";

  return out;
}

}  // namespace laq
