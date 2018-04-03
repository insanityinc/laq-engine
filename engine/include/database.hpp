/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_DATABASE_H_
#define ENGINE_INCLUDE_DATABASE_H_

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include "include/matrix.hpp"
#include "include/types.hpp"

namespace engine {

struct Database {
  std::string data_path;
  std::string database_name;
  std::map<std::string, std::map<std::string, std::vector<std::string>>> tables;

  Database(std::string dataPath, std::string databaseName);

  bool load();
  bool save();

  void createTable(std::string tableName,
                   std::map<std::string, std::vector<std::string>> attributes);

  // attributes: map of attribute name -> column in the file
  void copyFrom(std::string inFilePath,
                std::string outTable,
                std::map<Size, std::string> attributes,
                char delimiter);
};

}  // namespace engine

#endif  // ENGINE_INCLUDE_DATABASE_H_
