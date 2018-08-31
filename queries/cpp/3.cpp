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
  return args[0]=="BUILDING";
}
inline bool filter_var_c(std::vector<engine::Literal> args){
  return args[0]<"1995-03-15";
}
inline bool filter_var_h(std::vector<engine::Literal> args){
  return args[0]>"1995-03-15";
}
inline engine::Decimal lift_var_k(std::vector<engine::Decimal> args) {
  return args[0]*(1-args[1]);
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  engine::Database db(
    "data/la",
    "TPCH_1",
    false);

  engine::Bitmap *var_lineitem_orderkey =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "lineitem",
      "orderkey");
  engine::Bitmap *var_customer_mktsegment =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "customer",
      "mktsegment");
  engine::Bitmap *var_orders_custkey =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "orders",
      "custkey");
  engine::Bitmap *var_orders_orderdate =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "orders",
      "orderdate");
  engine::Bitmap *var_orders_shippriority =
    new engine::Bitmap(db.data_path,
      db.database_name,
      "orders",
      "shippriority");
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

  engine::FilteredBitVector *var_a_pred =
    new engine::FilteredBitVector(var_customer_mktsegment->nLabelBlocks);
  engine::FilteredBitVector *var_a =
    new engine::FilteredBitVector(var_customer_mktsegment->nBlocks);
  engine::FilteredBitVector *var_b =
    new engine::FilteredBitVector(var_orders_custkey->nBlocks);
  engine::FilteredBitVector *var_c_pred =
    new engine::FilteredBitVector(var_orders_orderdate->nLabelBlocks);
  engine::FilteredBitVector *var_c =
    new engine::FilteredBitVector(var_orders_orderdate->nBlocks);
  engine::FilteredBitVector *var_d =
    new engine::FilteredBitVector(var_b->nBlocks);
  engine::FilteredBitmap *var_e =
    new engine::FilteredBitmap(var_d->nBlocks);
  engine::FilteredBitmap *var_f =
    new engine::FilteredBitmap(var_e->nBlocks);
  engine::FilteredBitmap *var_g =
    new engine::FilteredBitmap(var_lineitem_orderkey->nBlocks);
  engine::FilteredBitVector *var_h_pred =
    new engine::FilteredBitVector(var_lineitem_shipdate->nLabelBlocks);
  engine::FilteredBitVector *var_h =
    new engine::FilteredBitVector(var_lineitem_shipdate->nBlocks);
  engine::FilteredBitmap *var_i =
    new engine::FilteredBitmap(var_g->nBlocks);
  engine::FilteredBitmap *var_j =
    new engine::FilteredBitmap(var_lineitem_orderkey->nBlocks);
  engine::DecimalVector *var_k =
    new engine::DecimalVector(var_lineitem_extendedprice->nBlocks);
  engine::FilteredDecimalMap *var_l =
    new engine::FilteredDecimalMap(var_j->nBlocks);
  engine::FilteredDecimalMapAcc *var_m_acc =
    new engine::FilteredDecimalMapAcc();
  engine::FilteredDecimalMap *var_m =
    new engine::FilteredDecimalMap(var_l->nBlocks);

  for (engine::Size i = 0; i < var_customer_mktsegment->nLabelBlocks; ++i) {
    var_customer_mktsegment->loadLabelBlock(i);
    var_a_pred->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_var_a,
      {
        *(var_customer_mktsegment->labels[i])
      },
      var_a_pred->blocks[i]);
    var_customer_mktsegment->deleteLabelBlock(i);
  }

  engine::Size A_nnz = 0;
  for(engine::Size i = 0; i < var_customer_mktsegment->nBlocks; ++i) {
    var_customer_mktsegment->loadBlock(i);
    var_a->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*var_a_pred, *(var_customer_mktsegment->blocks[i]), var_a->blocks[i]);

    A_nnz += var_a->blocks[i]->nnz;

    var_customer_mktsegment->deleteBlock(i);
  }

  std::cout << "A:\t\t" << A_nnz << std::endl;


  delete var_a_pred;

  for (engine::Size i = 0; i < var_orders_orderdate->nLabelBlocks; ++i) {
    var_orders_orderdate->loadLabelBlock(i);
    var_c_pred->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_var_c,
      {
        *(var_orders_orderdate->labels[i])
      },
      var_c_pred->blocks[i]);
    var_orders_orderdate->deleteLabelBlock(i);
  }

  engine::Size B_nnz = 0, C_nnz = 0, D_nnz = 0, E_nnz = 0, F_nnz = 0;

  for(engine::Size i = 0; i < var_orders_custkey->nBlocks; ++i) {
    var_orders_custkey->loadBlock(i);
    var_b->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*var_a, *(var_orders_custkey->blocks[i]), var_b->blocks[i]);
    var_orders_custkey->deleteBlock(i);

    B_nnz += var_b->blocks[i]->nnz;

    var_orders_orderdate->loadBlock(i);
    var_c->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*var_c_pred, *(var_orders_orderdate->blocks[i]), var_c->blocks[i]);
    var_d->blocks[i] = new engine::FilteredBitVectorBlock();

    C_nnz += var_c->blocks[i]->nnz;

    krao(*(var_b->blocks[i]), *(var_c->blocks[i]), var_d->blocks[i]);
    var_b->deleteBlock(i);
    var_c->deleteBlock(i);

    D_nnz += var_d->blocks[i]->nnz;

    var_e->blocks[i] = new engine::FilteredBitmapBlock();
    krao(*(var_d->blocks[i]), *(var_orders_orderdate->blocks[i]), var_e->blocks[i]);
    var_d->deleteBlock(i);
    var_orders_orderdate->deleteBlock(i);

    E_nnz += var_e->blocks[i]->nnz;

    var_orders_shippriority->loadBlock(i);
    var_f->blocks[i] = new engine::FilteredBitmapBlock();
    krao(*(var_e->blocks[i]), *(var_orders_shippriority->blocks[i]), var_f->blocks[i], var_orders_shippriority->nrows);
    var_e->deleteBlock(i);
    var_orders_shippriority->deleteBlock(i);

    F_nnz += var_f->blocks[i]->nnz;
  }

  std::cout << "B:\t\t" << B_nnz << std::endl;
  std::cout << "C:\t\t" << C_nnz << std::endl;
  std::cout << "D:\t\t" << D_nnz << std::endl;
  std::cout << "E:\t\t" << E_nnz << std::endl;
  std::cout << "F:\t\t" << F_nnz << std::endl;


  delete var_a;
  delete var_c_pred;

  for (engine::Size i = 0; i < var_lineitem_shipdate->nLabelBlocks; ++i) {
    var_lineitem_shipdate->loadLabelBlock(i);
    var_h_pred->blocks[i] = new engine::FilteredBitVectorBlock();
    filter(filter_var_h,
      {
        *(var_lineitem_shipdate->labels[i])
      },
      var_h_pred->blocks[i]);
    var_lineitem_shipdate->deleteLabelBlock(i);
  }

  engine::Size G_nnz = 0, H_nnz = 0, I_nnz = 0, J_nnz = 0, K_nnz = 0, L_nnz = 0;

  for(engine::Size i = 0; i < var_lineitem_orderkey->nBlocks; ++i) {
    var_lineitem_orderkey->loadBlock(i);
    var_lineitem_orderkey->loadBlock(i);
    var_g->blocks[i] = new engine::FilteredBitmapBlock();
    dot(*var_f, *(var_lineitem_orderkey->blocks[i]), var_g->blocks[i]);

    G_nnz += var_g->blocks[i]->nnz;

    var_lineitem_shipdate->loadBlock(i);
    var_h->blocks[i] = new engine::FilteredBitVectorBlock();
    dot(*var_h_pred, *(var_lineitem_shipdate->blocks[i]), var_h->blocks[i]);
    var_lineitem_shipdate->deleteBlock(i);

    H_nnz += var_h->blocks[i]->nnz;

    var_i->blocks[i] = new engine::FilteredBitmapBlock();
    krao(*(var_g->blocks[i]), *(var_h->blocks[i]), var_i->blocks[i]);
    var_g->deleteBlock(i);
    var_h->deleteBlock(i);

    I_nnz += var_i->blocks[i]->nnz;

    var_j->blocks[i] = new engine::FilteredBitmapBlock();
    krao(*(var_lineitem_orderkey->blocks[i]), *(var_i->blocks[i]), var_j->blocks[i], var_i->nrows);
    var_lineitem_orderkey->deleteBlock(i);
    var_i->deleteBlock(i);

    J_nnz += var_j->blocks[i]->nnz;

    var_lineitem_extendedprice->loadBlock(i);
    var_lineitem_discount->loadBlock(i);
    var_k->blocks[i] = new engine::DecimalVectorBlock();
    lift(lift_var_k,
      {
        *(var_lineitem_extendedprice->blocks[i]),
        *(var_lineitem_discount->blocks[i])
      },
      var_k->blocks[i]);
    var_lineitem_extendedprice->deleteBlock(i);
    var_lineitem_discount->deleteBlock(i);

    K_nnz += var_k->blocks[i]->nnz;

    var_l->blocks[i] = new engine::FilteredDecimalMapBlock();
    krao(*(var_j->blocks[i]), *(var_k->blocks[i]), var_l->blocks[i]);
    var_j->deleteBlock(i);
    var_k->deleteBlock(i);

    L_nnz += var_l->blocks[i]->nnz;

    sum(*var_l->blocks[i], var_m_acc);
    var_l->deleteBlock(i);
  }

  std::cout << "G:\t\t" << G_nnz << std::endl;
  std::cout << "H:\t\t" << H_nnz << std::endl;
  std::cout << "I:\t\t" << I_nnz << std::endl;
  std::cout << "J:\t\t" << J_nnz << std::endl;
  std::cout << "K:\t\t" << K_nnz << std::endl;
  std::cout << "L:\t\t" << L_nnz << std::endl;

  //(*var_m) = var_m_acc->getMatrix();

  for(auto& x : var_m_acc->map) {
    std::cout << x.second << std::endl;
  }

  delete var_m_acc;

  delete var_f;
  delete var_h_pred;

  //std::cout << (*var_m) << std::endl;

  delete var_m;

  /*for (const auto& kv : var_m_acc->map) {
    std::cout << kv.second << std::endl;
  }
  */
  std::cout << var_m_acc->map.size() << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  std::cout
    << "Completed in "
    << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()
    << " ns"
    << std::endl;

  return 0;
}
