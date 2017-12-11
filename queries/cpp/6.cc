/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Example of C++ main function
 * TPC-H Query 6
 */
#include "include/operators.h"

#ifndef DATA_PATH
  #define DATA_PATH "data"
#endif

inline bool filter_a(auto args) {
  return args[0] >= "1994-01-01" && args[0] <= "1995-01-01";
}

inline bool filter_b(auto args) {
  return args[0] >= 0.05 && args[0] <= 0.07;
}

inline auto map_d(auto args) {
  return args[0] * args[1];
}

inline bool filter_e(auto args) {
  return args[0] < 24;
}

int main() {
  using google::protobuf::Arena::CreateMessage;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  google::protobuf::Arena arena;

  engine::matrix *l_shipdate, *l_discount, *l_quantity, *l_extendedprice;
  engine::matrix *a, *b, *c, *d, *e, *f, *g, *h;

  l_shipdate = CreateMessage<engine::matrix>(&arena);
  l_discount = CreateMessage<engine::matrix>(&arena);
  l_quantity = CreateMessage<engine::matrix>(&arena);
  l_extendedprice = CreateMessage<engine::matrix>(&arena);

  a = CreateMessage<engine::matrix>(&arena);
  b = CreateMessage<engine::matrix>(&arena);
  c = CreateMessage<engine::matrix>(&arena);
  d = CreateMessage<engine::matrix>(&arena);
  e = CreateMessage<engine::matrix>(&arena);
  f = CreateMessage<engine::matrix>(&arena);
  g = CreateMessage<engine::matrix>(&arena);
  h = CreateMessage<engine::matrix>(&arena);

  sum_acc sum_h();

  loadMatrix(l_shipdate, DATA_PATH, "tpc-h", "lineitem", "shipdate");
  loadMatrix(l_discount, DATA_PATH, "tpc-h", "lineitem", "discount");
  loadMatrix(l_quantity, DATA_PATH, "tpc-h", "lineitem", "quantity");
  loadMatrix(l_extendedprice, DATA_PATH, "tpc-h", "lineitem", "extendedprice");

  for (int i = 0; i < l_shipdate.n_blocks; ++i) {

    // A = filter( l.shipdate >= "1994-01-01" AND l.shipdate <= "1995-01-01" )
    loadBlock(l_shipdate, DATA_PATH, &arena, i);
    filter(filter_a, l_shipdate.blocks[i], a.blocks[i]);
    delete_block(l_shipdate, i);

    // B = filter( l.discount >= 0.05 AND l.discount <= 0.07 )
    loadBlock(l_discount, DATA_PATH, &arena, i);
    filter(filter_b, l_discount.blocks[i], b.blocks[i]);

    // C = hadamard( A, B )
    krao(a, b, c, 1, CSC = false, move = true);
    delete_block(a, i);
    delete_block(b, i);

    // D = map( l.extendedprice * l.discount )
    loadBlock(l_extendedprice, DATA_PATH, &arena, i);
    map(map_d, d.blocks[i], l_extendedprice.blocks[i], l_discount.blocks[i]);
    delete_block(l_extendedprice, i);
    delete_block(l_discount, i);

    // E = filter( l.quantity < 24 )
    loadBlock(l_quantity, DATA_PATH, &arena, i);
    filter(filter_c, l_quantity.blocks[i], c.blocks[i]);
    delete_block(l_quantity, i);

    // F = hadamard( D, E )
    krao(d, e, f, 1, CSC = false, move = true);
    delete_block(d, i);
    delete_block(e, i);

    // G = hadamard( C, F )
    krao(c, f, g, 1, CSC = false, move = true);
    delete_block(c, i);
    delete_block(f, i);

    // H = sum( G )
    sum(g, sum_h);
    delete_block(g, i);

  }

  acc_to_matrix(sum_h, h);

  // Print or store H

  return 0;
}
