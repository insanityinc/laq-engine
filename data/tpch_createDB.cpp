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
  db.createTable("customer", {
    {"custkey", {"dimension", "customer", "custkey"} },
    {"mktsegment", {"dimension", "customer", "mktsegment"}}
  });

  db.createTable("part", {
    {"partkey", {"dimension", "part", "partkey"} },
    {"type", {"dimension", "part", "type"}}
  });

  db.createTable("orders", {
    {"orderkey", {"dimension", "orders", "orderkey"} },
    {"custkey", {"dimension", "customer", "custkey"} },
    {"orderdate", {"dimension", "orders", "orderdate"} },
    {"shippriority", {"dimension", "orders", "shippriority"} }
  });

  db.createTable("lineitem", {
    {"orderkey", {"dimension", "orders", "orderkey"} },
    {"partkey", {"dimension", "part", "partkey"} },
    {"quantity", {"measure"} },
    {"extendedprice", {"measure"} },
    {"discount", {"measure"} },
    {"shipdate", {"dimension", "lineitem", "shipdate"} }
  });

  // copy from "path" to "table" {file column, "attribute"},
  // separating by "delimiter"

  db.copyFrom(std::string(DBGEN_DATA_PATH) + "/" + std::string(DATASET) + "/customer.tbl",
    "customer",
    {
      {0, "custkey"},
      {6, "mktsegment"}
    },
    '|');

  db.copyFrom(std::string(DBGEN_DATA_PATH) + "/" + std::string(DATASET) + "/part.tbl",
    "part",
    {
      {0, "partkey"},
      {4, "type"}
    },
    '|');

  db.copyFrom(std::string(DBGEN_DATA_PATH) + "/" + std::string(DATASET) + "/orders.tbl",
    "orders",
    {
      {0, "orderkey"},
      {1, "custkey"},
      {4, "orderdate"},
      {7, "shippriority"}
    },
    '|');

  db.copyFrom(std::string(DBGEN_DATA_PATH) + "/" + std::string(DATASET) + "/lineitem.tbl",
    "lineitem",
    {
      {0, "orderkey"},
      {1, "partkey"},
      {4, "quantity"},
      {5, "extendedprice"},
      {6, "discount"},
      {10, "shipdate"}
    },
    '|');
}
