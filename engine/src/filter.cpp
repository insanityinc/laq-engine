/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */
#include "include/filter.hpp"
#include <vector>
#include "include/types.hpp"

namespace engine {

void filter(bool(*f)(std::vector<Decimal>),
            const std::vector<DecimalVectorBlock>& in,
            FilteredBitVectorBlock* out) {
  std::vector<Decimal> v(in.size());
  Size i, nnz = 0;
  for (i = 0; i < in[0].nnz; ++i) {
    out->cols[i] = nnz;
    for (Size j = 0; j < in.size(); ++j) {
      v[j] = in[j].values[i];
    }
    if ((*f)(v)) {
      ++nnz;
    }
  }
  out->cols[i] = nnz;
  out->nnz = nnz;
}

void filter(bool(*f)(std::vector<Literal>),
            const std::vector<LabelBlock>& in,
            FilteredBitVectorBlock* out) {
  std::vector<Literal> v(in.size());
  Size i, nnz = 0;
  for (i = 0; i < in[0].nnz; ++i) {
    out->cols[i] = nnz;
    for (Size j = 0; j < in.size(); ++j) {
      v[j] = in[j].labels[i];
    }
    if ((*f)(v)) {
      ++nnz;
    }
  }
  out->cols[i] = nnz;
  out->nnz = nnz;
}

void filter(bool(*f)(std::vector<Decimal>),
            const std::vector<DecimalVectorBlock>& in,
            const FilteredBitVectorBlock& iter,
            FilteredBitVectorBlock* out) {
  std::vector<Decimal> v(in.size());
  Size i, nnz = 0;
  for (i = 0; i < iter.cols.size(); ++i) {
    out->cols[i] = nnz;
    if (iter.cols[i+1] > iter.cols[i]) {
      for (Size j = 0; j < in.size(); ++j) {
        v[j] = in[j].values[i];
      }
      if ((*f)(v)) {
        ++nnz;
      }
    }
  }
  out->cols[i] = nnz;
  out->nnz = nnz;
}

}  // namespace engine
