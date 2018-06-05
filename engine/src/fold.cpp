/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */
#include <map>
#include "include/fold.hpp"

namespace engine {

void sum(const FilteredDecimalVectorBlock& in,
         Decimal *acc) {
  for (auto& v : in.values) {
    (*acc) += v;
  }
}

void sum(const FilteredDecimalMapBlock& in,
         FilteredDecimalMapAcc *acc) {

  //std::map<MultiPrecision,Decimal> map;
  for (Size i=0; i < in.values.size(); ++i) {
    // operator [] searches for an element
    // and initializes it if it does not exist
    acc->map[in.rows[i]] += in.values[i];
  }

  /*
  this is the part of getMatrix()

  void getMatrix(FilteredDecimalMapAcc& acc, FilteredDecimalMap *mat) {
    Size nnz = 0;
    for (const auto& kv : acc.map) {
      mat->rows[nnz] = kv.first;
      mat->values[nnz] = kv.second;
      ++nnz;
    }
    mat->nnz = nnz;
    mat->cols = {0, nnz};
  }
  */

}

}  // namespace engine
