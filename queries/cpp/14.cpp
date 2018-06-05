/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 *
 * This query was automatically generated
 */
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "include/block.hpp"
#include "include/database.hpp"
#include "include/dot.hpp"
#include "include/filter.hpp"
#include "include/fold.hpp"
#include "include/functions.hpp"
#include "include/krao.hpp"
#include "include/lift.hpp"
#include "include/matrix.hpp"
#include "include/types.hpp"

inline bool filter_var_a(std::vector<engine::Literal> args){
  return args[0]>="1995-09-01"&&args[0]<"1995-10-01";
}
inline engine::Decimal lift_var_b(std::vector<engine::Decimal> args) {
  return args[0]*(1-args[1]);
}
inline bool filter_var_d(std::vector<engine::Literal> args){
  return match(args[0],"PROMO.*");
}
inline engine::Decimal lift_var_i(std::vector<engine::Decimal> args) {
  return 100.00*args[0]/args[1];
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  engine::Database db(
    "data/la",
    "TPCH_1",
    false);

  engine::Bitmap *var_lineitem_shipdate =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "lineitem",
      "shipdate");
  engine::DecimalVector *var_lineitem_extendedprice =
    new engine::DecimalVector(db.data_path,
      db.database_name,
      "lineitem",
      "extendedprice");
  engine::DecimalVector *var_lineitem_discount =
    new engine::DecimalVector(db.data_path,
      db.database_name,
      "lineitem",
      "discount");
  engine::Bitmap *var_part_type =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "part",
      "type");
  engine::Bitmap *var_lineitem_partkey =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "lineitem",
      "partkey");

  engine::FilteredBitVector *var_a_pred =
    new engine::FilteredBitVector(var_lineitem_shipdate->nLabelBlocks);
  engine::FilteredBitVector *var_a =
    new engine::FilteredBitVector(var_lineitem_shipdate->nBlocks);
  engine::DecimalVector *var_b =
    new engine::DecimalVector(var_lineitem_extendedprice->nBlocks);
  engine::FilteredDecimalVector *var_c =
    new engine::FilteredDecimalVector(var_a->nBlocks);
  engine::FilteredBitVector *var_d_pred =
    new engine::FilteredBitVector(var_part_type->nLabelBlocks);
  engine::FilteredBitVector *var_d =
    new engine::FilteredBitVector(var_part_type->nBlocks);
  engine::FilteredBitVector *var_e =
    new engine::FilteredBitVector(var_lineitem_partkey->nBlocks);
  engine::FilteredDecimalVector *var_f =
    new engine::FilteredDecimalVector(var_c->nBlocks);
  engine::Decimal *var_g =
    new engine::Decimal();
  engine::Decimal *var_h =
    new engine::Decimal();
  engine::Decimal *var_i =
    new engine::Decimal();

  for (engine::Size i = 0; i < var_lineitem_shipdate->nLabelBlocks; ++i) {
    var_lineitem_shipdate->loadLabelBlock(i);
    var_a_pred->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_var_a,
      {
        *(var_lineitem_shipdate->labels[i])
      },
      var_a_pred->blocks[i]);
    var_lineitem_shipdate->deleteLabelBlock(i);
  }

  for (engine::Size i = 0; i < var_part_type->nLabelBlocks; ++i) {
    var_part_type->loadLabelBlock(i);
    var_d_pred->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_var_d,
      {
        *(var_part_type->labels[i])
      },
      var_d_pred->blocks[i]);
    var_part_type->deleteLabelBlock(i);
  }

  for(engine::Size i = 0; i < var_part_type->nBlocks; ++i) {
    var_part_type->loadBlock(i);
    var_d->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*var_d_pred, *(var_part_type->blocks[i]), var_d->blocks[i]);
    var_part_type->deleteBlock(i);
  }

  delete var_d_pred;

  for(engine::Size i = 0; i < var_lineitem_shipdate->nBlocks; ++i) {
    var_lineitem_shipdate->loadBlock(i);
    var_a->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*var_a_pred, *(var_lineitem_shipdate->blocks[i]), var_a->blocks[i]);
    var_lineitem_shipdate->deleteBlock(i);
    var_lineitem_extendedprice->loadBlock(i);
    var_lineitem_discount->loadBlock(i);
    var_b->blocks[i] = new engine::DecimalVectorBlock();
    lift(lift_var_b,
      {
        *(var_lineitem_extendedprice->blocks[i]),
        *(var_lineitem_discount->blocks[i])
      },
      var_b->blocks[i]);
    var_lineitem_extendedprice->deleteBlock(i);
    var_lineitem_discount->deleteBlock(i);

    var_c->blocks[i] = new engine::FilteredDecimalVectorBlock();
    krao(*(var_a->blocks[i]), *(var_b->blocks[i]), var_c->blocks[i]);
    var_a->deleteBlock(i);
    var_b->deleteBlock(i);

    var_lineitem_partkey->loadBlock(i);
    var_e->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*var_d, *(var_lineitem_partkey->blocks[i]), var_e->blocks[i]);
    var_lineitem_partkey->deleteBlock(i);

    var_f->blocks[i] = new engine::FilteredDecimalVectorBlock();
    krao(*(var_c->blocks[i]), *(var_e->blocks[i]), var_f->blocks[i]);
    var_e->deleteBlock(i);

    sum(*var_f->blocks[i], var_g);
    var_f->deleteBlock(i);

    sum(*var_c->blocks[i], var_h);
    var_c->deleteBlock(i);
  }

  delete var_a_pred;
  delete var_d;


  (*var_i) = lift_var_i(
      {
        *(var_g),
        *(var_h)
      });

  std::cout << (*var_i) << std::endl;

  delete var_g;

  delete var_h;

  delete var_i;

  auto end = std::chrono::high_resolution_clock::now();
  std::cout
    << "Completed in "
    << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()
    << " ns"
    << std::endl;

  return 0;
}
