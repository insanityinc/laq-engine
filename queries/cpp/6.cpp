/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 *
 * This query was automatically generated
 */
#include <iostream>
#include <vector>
#include "include/block.hpp"
#include "include/database.hpp"
#include "include/dot.hpp"
#include "include/filter.hpp"
#include "include/fold.hpp"
#include "include/krao.hpp"
#include "include/lift.hpp"
#include "include/matrix.hpp"
#include "include/types.hpp"

inline bool filter_a(std::vector<engine::Literal> args) {
  return args[0] >= "1994-01-01" && args[0] < "1995-01-01";
}

inline bool filter_b(std::vector<engine::Decimal> args) {
  return args[0] >= 0.05 && args[0] <= 0.07;
}

inline bool filter_c(std::vector<engine::Decimal> args) {
  return args[0] < 24;
}

inline engine::Decimal lift_f(std::vector<engine::Decimal> args) {
  return args[0] * args[1];
}

int main() {
  engine::Database db(
    "data/la",
    "TPCH_1",
    false);

  engine::Bitmap *lineitem_shipdate =
    new engine::Bitmap(db.data_path,
                       db.database_name,
                       "lineitem",
                       "shipdate");
  engine::DecimalVector *lineitem_discount =
    new engine::DecimalVector(db.data_path,
                              db.database_name,
                              "lineitem",
                              "discount");
  engine::DecimalVector *lineitem_quantity =
    new engine::DecimalVector(db.data_path,
                              db.database_name,
                              "lineitem",
                              "quantity");
  engine::DecimalVector *lineitem_extendedprice =
    new engine::DecimalVector(db.data_path,
                              db.database_name,
                              "lineitem",
                              "extendedprice");

  engine::FilteredBitVector *a_pred =
    new engine::FilteredBitVector(lineitem_shipdate->nLabelBlocks);
  engine::FilteredBitVector *a =
    new engine::FilteredBitVector(lineitem_shipdate->nBlocks);
  engine::FilteredBitVector *b =
    new engine::FilteredBitVector(lineitem_discount->nBlocks);
  engine::FilteredBitVector *c =
    new engine::FilteredBitVector(lineitem_quantity->nBlocks);
  engine::FilteredBitVector *d =
    new engine::FilteredBitVector(a->nBlocks);
  engine::FilteredBitVector *e =
    new engine::FilteredBitVector(c->nBlocks);
  engine::DecimalVector *f =
    new engine::DecimalVector(lineitem_extendedprice->nBlocks);
  engine::FilteredDecimalVector *g =
    new engine::FilteredDecimalVector(e->nBlocks);
  engine::Decimal *h =
    new engine::Decimal();

  #pragma omp parallel for
  for (engine::Size i = 0; i < lineitem_shipdate->nLabelBlocks; ++i) {
    // A = filter( lineitem.shipdate >= "1994-01-01"
    //             AND lineitem.shipdate < "1995-01-01" )
    lineitem_shipdate->loadLabelBlock(i);
    a_pred->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_a, {*(lineitem_shipdate->labels[i])}, a_pred->blocks[i]);
    lineitem_shipdate->deleteLabelBlock(i);
  }

  #pragma omp parallel for
  for (engine::Size i = 0; i < lineitem_shipdate->nBlocks; ++i) {
    // A = filter( lineitem.shipdate >= "1994-01-01"
    //             AND lineitem.shipdate < "1995-01-01" )
    lineitem_shipdate->loadBlock(i);
    a->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*a_pred, *(lineitem_shipdate->blocks[i]), a->blocks[i]);
    lineitem_shipdate->deleteBlock(i);

    // B = filter( lineitem.discount >= 0.05 AND lineitem.discount <= 0.07 )
    lineitem_discount->loadBlock(i);
    b->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_b, {*(lineitem_discount->blocks[i])}, b->blocks[i]);

    // C = filter( lineitem.quantity < 24 )
    lineitem_quantity->loadBlock(i);
    c->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_c, {*(lineitem_quantity->blocks[i])}, c->blocks[i]);
    lineitem_quantity->deleteBlock(i);

    // D = hadamard( A, B )
    d->blocks[i] = new engine::FilteredBitVectorBlock();
    krao(*(a->blocks[i]), *(b->blocks[i]), d->blocks[i]);
    delete a->blocks[i];
    delete b->blocks[i];

    // E = hadamard( C, D )
    e->blocks[i] = new engine::FilteredBitVectorBlock();
    krao(*(c->blocks[i]), *(d->blocks[i]), e->blocks[i]);
    delete c->blocks[i];
    delete d->blocks[i];

    // F = lift( lineitem.extendedprice * lineitem.discount )
    lineitem_extendedprice->loadBlock(i);
    f->blocks[i] = new engine::DecimalVectorBlock();
    lift(lift_f,
         {*(lineitem_discount->blocks[i]),
         *(lineitem_extendedprice->blocks[i])},
         f->blocks[i]);
    lineitem_extendedprice->deleteBlock(i);
    lineitem_discount->deleteBlock(i);

    // G = hadamard( E, F )
    g->blocks[i] = new engine::FilteredDecimalVectorBlock();
    krao(*(e->blocks[i]), *(f->blocks[i]), g->blocks[i]);
    delete e->blocks[i];
    delete f->blocks[i];

    // H = sum( G )
    sum(*(g->blocks[i]), h);
  }

  delete a_pred;

  std::cout << (*h) << std::endl;

  return 0;
}
