/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */
#include "include/lift.hpp"
#include <vector>

namespace engine {

// function must be inline, needs modification?
void lift(Decimal(*f)(std::vector<Decimal>),
          const std::vector<DecimalVectorBlock>& in,
          DecimalVectorBlock* out) {
  std::vector<Decimal> v(in.size());

  for (Size i = 0; i < in[0].nnz; ++i) {
    for (Size j = 0; j < in.size(); ++j) {
      v[j] = in[j].values[i];
    }
    out->values[i] = (*f)(v);
  }
  out->nnz = in[0].nnz;
}

void lift(Decimal(*f)(std::vector<Decimal>),
          const std::vector<DecimalVectorBlock>& in,
          const FilteredBitVectorBlock& iter,
          FilteredDecimalVectorBlock* out) {
  std::vector<Decimal> v(in.size());

  Size i, nnz = 0;
  for (i = 0; i < iter.cols.size(); ++i) {
    out->cols[i] = nnz;
    if (iter.cols[i+1] > iter.cols[i]) {
      for (Size j = 0; j < in.size(); ++j) {
        v[j] = in[j].values[i];
      }
      out->values[nnz] = (*f)(v);
      ++nnz;
    }
  }
  out->cols[i] = nnz;
  out->nnz = nnz;
}

}  // namespace engine
