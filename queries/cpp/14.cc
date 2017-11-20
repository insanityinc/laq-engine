/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 * Example of C++ main function
 * TPC-H Query 14
 *
 * Query in SQL format:
 *
 *  select
 *    100.00 * sum(case
 *      when p_type like 'PROMO%'
 *        then l_extendedprice * (1 - l_discount)
 *      else 0
 *    end) / sum(l_extendedprice * (1 - l_discount)) as promo_revenue
 *  from
 *    lineitem,
 *    part
 *  where
 *    l_partkey = p_partkey
 *    and l_shipdate >= date ''
 *    and l_shipdate < date '' + interval '1' month;
 *
 * Query in DSL format:
 *
 *  A = filter( match( p.type , ".+PROMO.+" ) )
 *  B = dot( A, l.partkey )
 *  C = filter( l.shipdate >= "" )
 *  D = filter( l.shipdate < "" )
 *  E = hadamard( C, D )
 *  F = hadamard( B, E )
 *  G = map( l.extendedprice*(1-l.discount) )
 *  H = hadamard( F, G )
 *  I = sum( G )
 *  J = sum( H )
 *  K = map( 100.00 * J / I )
 *  return ( K )
 *
 * Query dependence graph:
 *
 *  p.type --> A --
 *                 |--> B ---------
 *  l.partkey -----                |
 *                                 |--> F --
 *                --> C --         |        |
 *  l.shipdate --|        |--> E --         |
 *                --> D --                  |--> H --> J --
 *                                          |              |
 *  l.extendedprice --                      |              |--> K
 *                    |--> G ---------------               |
 *  l.discount -------         |                           |
 *                              --> I ---------------------
 */

inline bool filter_a(auto p_type) {
  return match(p_type, ".+PROMO.+");
}

inline bool filter_c(auto l_shipdate) {
  return l_shipdate >= "1995-09-01";
}

inline bool filter_d(auto l_shipdate) {
  return l_shipdate < "1995-10-01";
}

inline auto map_g(auto l_extendedprice, auto l_discount) {
  return l_extendedprice * (1 - l_discount);
}

inline auto map_k(auto j, auto i) {
  return 100.00 * j / i;
}

int main() {
  /** Como B depende de A, calcula-se A
    * Pré-condição: --
    * Pós-condição: A calculado e guardado na RAM
    */

  // in:
  Matrix p_type;
  // out:
  Matrix a;

  for (int i = 0; i < p_type.n_blocks; ++i) {
    p_type.load(i);
    filter(/*filter:*/ filter_a,
           /*in:*/ p_type.blocks[i],
           /*out:*/ a.blocks[i]);
    p_type.delete(i);
  }


  /** Pode calcular-se H em stream, mas como I depende de G, calcular G
    * Pré-condição: --
    * Pós-condição: G calculado e guardado na RAM
    */

  // in:
  Matrix l_extendedprice, l_discount;
  // out:
  Matrix g;

  for (int i = 0; i < l_discount.n_blocks; ++i) {
    // load:
    l_extendedprice.load(i);
    l_discount.load(i);
    // compute:
    map(/*map:*/ map_g,
        /*out:*/ g.blocks[i],
        /*arguments:*/ l_extendedprice.blocks[i], l_discount.blocks[i]);
    // delete:
    l_extendedprice.delete(i);
    l_discount.delete(i);
  }


  /** J depende de H, calcular H com stream de B,C e D
    * Pré-condição: A e G em RAM
    * Pós-condição: H calculado e guardado na RAM, A apagado
    */

  // in:
  Matrix l_partkey, l_shipdate;
  // tmp:
  Matrix b, c, d, e, f;
  // out:
  Matrix h;

  for (int i = 0; i < l_partkey.n_blocks; ++i) {
    // load:
    l_partkey.load(i);
    l_shipdate.load(i);
    // compute:
    dot(a, l_partkey.blocks[i], b.blocks[i]);
    filter(/*filter:*/ filter_c,
           /*in:*/ l_shipdate.blocks[i],
           /*out:*/ c.blocks[i]);
    filter(/*filter:*/ filter_d,
           /*in:*/ l_shipdate.blocks[i],
           /*out:*/ d.blocks[i]);
    hadamard(/*in:*/ c.blocks[i], d.blocks[i],
             /*out:*/ d.blocks[i]);
    hadamard(/*in:*/ b.blocks[i], e.blocks[i],
             /*out:*/ f.blocks[i]);
    hadamard(/*in:*/ f.blocks[i], g.blocks[i],
             /*out:*/ h.blocks[i]);
    // delete:
    l_partkey.delete(i);
    l_shipdate.delete(i);
    b.delete(i);
    c.delete(i);
    d.delete(i);
    e.delete(i);
    f.delete(i);
  }

  a.deleteAll();


  /** Calcular J e libertar H (ou calcular I e libertar G)
    * Pré-condição: H em RAM
    * Pós-condição: J calculado e guardado na RAM, H apagado
    */

  // in: --
  // out:
  Matrix j;

  // load: --
  // compute:
  sum(/*in:*/ h,
      /*out:*/ j);
  // delete:
  h.deleteAll();


  /** Calcular I e libertar G (ou calcular J e libertar H)
    * Pré-condição: G em RAM
    * Pós-condição: I calculado e guardado na RAM, G apagado
    */

  // in: --
  // out:
  Matrix i;

  // load: --
  // compute:
  sum(/*in:*/ g,
      /*out:*/ i);
  // delete:
  g.deleteAll();


  /** Calcular K e libertar I e J
    * Pré-condição: I e J em RAM
    * Pós-condição: K calculado e guardado na RAM, I e J apagados
    */

  // in: --
  // out:
  Matrix k;

  for (int i = 0; i < j.n_blocks; ++i) {
    // load: --
    // compute:
    map(/*map:*/ map_k,
        /*out:*/ k.blocks[i],
        /*arguments:*/ j.blocks[i], i.blocks[i]);
    // delete:
  }

  i.deleteAll();
  j.deleteAll();

  // Print or store K

  return 0;
}
