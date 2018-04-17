/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 *
 * Create TPC-H Database Schema
 */
#include <iostream>
#include <string>
#include "include/database.hpp"

#ifndef DATA_PATH
  #define DATA_PATH "data/la"
#endif

#ifndef DBGEN_DATA_PATH
  #define DBGEN_DATA_PATH "data/dbgen"
#endif

#ifndef DATASET
  #define DATASET "1"
#endif
#ifndef BSIZE
  #define BSIZE 1024
#endif

int main() {
  // check if database exists, if not create it
  // else return table exists
  engine::Database db(DATA_PATH,
                      std::string("TPCH_") + DATASET,
                      true);

  // check if table exists, if not create it
  // else return table exists
  db.createTable("lineitem", {
    {"quantity", {"measure"} },
    {"extendedprice", {"measure"} },
    {"discount", {"measure"} },
    {"shipdate", {"dimension", "lineitem", "shipdate"} }
  });

  // copy from "path" to "table" {file column, "attribute"},
  // separating by "delimiter"

  db.copyFrom(std::string(DBGEN_DATA_PATH) + "/" + std::string(DATASET) + "/lineitem.tbl",
    "lineitem",
    {
      {4, "quantity"},
      {5, "extendedprice"},
      {6, "discount"},
      {10, "shipdate"}
    },
    '|');
}
