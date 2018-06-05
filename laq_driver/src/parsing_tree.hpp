/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef LAQ_DRIVER_SRC_PARSING_TREE_H_
#define LAQ_DRIVER_SRC_PARSING_TREE_H_
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "include/database.hpp"
#include "include/laq_driver.hpp"

namespace laq {

typedef
std::map<std::tuple<std::string, std::string, std::string>, std::string>
MatrixTypeMap;

typedef
std::map<std::string, std::string>
AttributeTypes;

typedef
std::map<std::string, std::tuple<std::string, engine::Size, engine::Size>>
AttributeAlloc;

class driver::ParsingTree {
 public:
  ParsingTree();

  void insertStatement(const std::string& lvar,
                       const std::string& op,
                       const std::vector<std::string>& rvars,
                       const std::string& expr);

  std::string getQuery();

  std::string toCpp(engine::Database& db);

 private:
  struct ForLoop {
    std::string head;
    std::string tail;
    std::vector<std::string> deleteVars;
    ForLoop(std::string h, std::string t) : head(h), tail(t) {}
  };

  std::vector<ForLoop>
  getForLoops(AttributeTypes& attrTypes,
              AttributeAlloc& attrAlloc,
              const std::string& var);

  std::pair<std::string, std::string>
  declareTmpVars(MatrixTypeMap& matTypeMap,
                 AttributeTypes& attrTypes,
                 AttributeAlloc& attrAlloc,
                 const std::string& var);
  std::string
  declareAttrVars(AttributeTypes& attrTypes,
                  AttributeAlloc& attrAlloc,
                  const std::string& var);
  std::string
  predicates(AttributeTypes& attrTypes,
             AttributeAlloc& attrAlloc,
             const std::string& var);

  void
  getAttrAlloc(MatrixTypeMap& matTypeMap,
               AttributeTypes& attrTypes,
               AttributeAlloc& attrAlloc,
               const std::string& var);

  class Statement;

  std::map<std::string, Statement> tree;
  std::vector<std::string> vars;
};  // class ParsingTree

}  // namespace laq

#endif  // LAQ_DRIVER_SRC_PARSING_TREE_H_
