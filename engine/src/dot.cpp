/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */
#include "include/dot.hpp"

namespace engine {

void dot(const FilteredBitVector& A,
         const BitmapBlock& B,
         FilteredBitVectorBlock *C) {
  Size i, nnz = 0;
  for (i = 0; i < B.nnz; ++i) {
    C->cols[i] = nnz;

    auto row = B.rows[i];
    auto r = row % (A.blocks[0]->cols.size() - 1);
    auto b = row / (A.blocks[0]->cols.size() - 1);

    if (A.blocks[b]->cols[r+1] > A.blocks[b]->cols[r]) {
      ++nnz;
    }
  }
  C->cols[i] = nnz;
  C->nnz = nnz;
}

}  // namespace engine
