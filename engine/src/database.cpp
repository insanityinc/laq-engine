/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */

#include <cereal/archives/binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <experimental/filesystem>
#include <map>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include "include/matrix.hpp"
#include "include/database.hpp"
#include "include/types.hpp"

#include <iostream>
using namespace std;

namespace engine {

Database::Database(std::string dataPath, std::string databaseName) {
  data_path = dataPath;
  database_name = databaseName;

  mode_t mode = 0733;
  int error = 0;
  #if defined(_WIN32)
    error = _mkdir((dataPath + "/" + databaseName).c_str());
  #else
    error = mkdir((dataPath + "/" + databaseName).c_str(),mode);
  #endif
  if (error != 0) {
    // handle error here
  }

  this->save();
}

bool Database::load() {
  std::ifstream ifs(data_path + "/" + database_name + "/meta.dat");
  if (ifs.is_open()) {
    {
      cereal::BinaryInputArchive iarchive(ifs);
      iarchive(data_path, database_name, tables);
    }
    ifs.close();
    return true;
  }
  return false;
}

bool Database::save() {
  std::ofstream ofs(data_path + "/" + database_name + "/meta.dat");
  if (ofs.is_open()) {
    {
      cereal::BinaryOutputArchive oarchive(ofs);
      oarchive(data_path, database_name, tables);
    }
    ofs.close();
    return true;
  }
  return false;
}

void Database::createTable(std::string tableName,
                           std::map<std::string,
                           std::vector<std::string>> attributes) {
  tables[tableName] = attributes;

  std::string path = data_path + "/" + database_name + "/" + tableName;
  mode_t mode = 0733;
  int error = 0;
  #ifdef _WIN32
    error = _mkdir(path.c_str());
  #else
    error = mkdir(path.c_str(),mode);
  #endif

  for (const auto& attr : attributes) {
    #ifdef _WIN32
      error = _mkdir((path + "/" + attr.first).c_str());
      error = _mkdir((path + "/" + attr.first + "/blocks").c_str());
    #else
      error = mkdir((path + "/" + attr.first).c_str(),mode);
      error = mkdir((path + "/" + attr.first + "/blocks").c_str(),mode);
    #endif
    if (attr.second[0] == "measure") {
      DecimalVector v(data_path, database_name, tableName, attr.first);
      v.save();
    } else if (attr.second[0] == "dimension") {
      #ifdef _WIN32
        error = _mkdir((path + "/" + attr.first + "/labels").c_str());
      #else
        error = mkdir((path + "/" + attr.first + "/labels").c_str(),mode);
      #endif
      Bitmap b(data_path, database_name, tableName, attr.first);
      b.save();
    }
  }
  if (error != 0) {
    // handle error here
  }
  this->save();
}

void Database::copyFrom(std::string inFilePath,
                        std::string outTable,
                        std::map<Size, std::string> attributes,
                        char delimiter) {
  std::ifstream input(inFilePath);

  if (input.is_open()) {

    std::string line;

    std::map<std::string, Matrix*> matrices;

    for (const auto& attr : attributes) {

      if (tables[outTable][attr.second][0] == "measure") {
        matrices[attr.second] = new DecimalVector(data_path,
                                                  database_name,
                                                  outTable,
                                                  attr.second);
      } else if (tables[outTable][attr.second][0] == "dimension") {
        matrices[attr.second] = new Bitmap(data_path,
                                           database_name,
                                           outTable,
                                           attr.second);
      }

    }


    while (std::getline(input, line)) {
      std::string value;
      std::stringstream ss(line);

      for (Size i=0; std::getline(ss, value, delimiter); ++i) {

        if (attributes.find(i) != attributes.end()) {

          if (tables[outTable][attributes[i]][0] == "measure") {

            Decimal dec = std::stod(value);
            DecimalVector *v = (DecimalVector*) matrices[attributes[i]];
            v->insert((Decimal) dec);

          } else if (tables[outTable][attributes[i]][0] == "dimension") {

            Bitmap *b = (Bitmap*) matrices[attributes[i]];
            b->insert((Literal) value);
          }
        }
      }
    }

    for (const auto& mat : matrices) {
      if (tables[outTable][mat.first][0] == "measure") {
        DecimalVector *v = (DecimalVector*) mat.second;
        v->saveLastBlock();
      } else if (tables[outTable][mat.first][0] == "dimension") {
        Bitmap *b = (Bitmap*) mat.second;
        b->saveLastBlock();
        b->saveLastLabelBlock();
        b->saveLabelHash();
      }
      mat.second->save();
    }

  }
}

}  // namespace engine
