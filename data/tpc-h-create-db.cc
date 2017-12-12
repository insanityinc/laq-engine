/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Create TPC-H Database Schema
 */
#include "include/operators.h"

#ifndef DATA_PATH
  #define DATA_PATH "data"
#endif

int main() {
  using google::protobuf::Arena::CreateMessage;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  google::protobuf::Arena arena;

  database* db = google::protobuf::Arena::CreateMessage<database>(&arena);

  engine::createDatabase(db, DATA_PATH, "tpc-h");

  // table lineitem
  std::map<std::string, engine::matrix_Type> lineitem;
  lineitem["extendedprice"] = engine::matrix_Type_MEASURE;
  lineitem["discount"] = engine::matrix_Type_MEASURE;
  lineitem["shipdate"] = engine::matrix_Type_DIMENSION;
  lineitem["orderkey"] = engine::matrix_Type_DIMENSION;

  std::map<std::string, engine::matrix_Type> lineitem_fks;
  lineitem_fks["orderkey"] = std::make_tuple("orders", "orderkey");

  engine::createTable(database* db,
                      "lineitem",
                      lineitem,
                      "TODO_COMPOSITE_KEYS",
                      lineitem_fks,
                      &arena);

