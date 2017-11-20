/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Example of C++ main function
 * TPC-H Query 6
 */
#include "include/operators.h"

inline bool filter_a(auto l_shipdate) {
  return l.shipdate >= "1994-01-01" && l.shipdate <= "1995-01-01";
}

inline bool filter_b(auto l_discount) {
  return l_discount >= 0.05 && l_discount <= 0.07;
}

inline bool filter_c(auto l_quantity) {
  return l_quantity < 24;
}

inline auto map_f(auto l_extendedprice, auto l_discount) {
  return l.extendedprice * l.discount;
}

int main() {
  Matrix l_shipdate;
  Matrix l_discount;
  Matrix l_quantity;
  Matrix l_extendedprice;

  Matrix a, b, c, d, e, f, g, h;

  for (int i = 0; i < l_shipdate.n_blocks; ++i) {
    l_shipdate.load(i);
    filter(filter_a, l_shipdate.blocks[i], a.blocks[i]);
    l_shipdate.delete(i);

    l_discount.load(i);
    filter(filter_b, l_discount.blocks[i], b.blocks[i]);
    l_quantity.load(i);
    filter(filter_c, l_quantity.blocks[i], c.blocks[i]);
    l_quantity.delete(i);

    krao(a, b, d, 1, CSC = false, move = true);
    a.delete(i);
    b.delete(i);

    krao(c, d, e, 1, CSC = false, move = true);
    c.delete(i);
    d.delete(i);

    l_extendedprice.load(i);
    map(map_f, f.blocks[i], l_extendedprice.blocks[i], l_discount.blocks[i]);

    krao(e, f, g, 1, CSC = false, move = true);
    c.delete(i);
    d.delete(i);
  }

  for (int i = 0; i < g.n_blocks; ++i) {
    sum(g, h);
    g.delete(i);
  }

  // Print or store H

  return 0;
}
