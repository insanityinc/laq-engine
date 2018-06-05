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

    auto b_row = B.rows[i];
    auto a_col = b_row % (A.blocks[0]->cols.size() - 1);
    auto block = b_row / (A.blocks[0]->cols.size() - 1);

    if (A.blocks[block]->cols[a_col+1] > A.blocks[block]->cols[a_col]) {
      ++nnz;
    }
  }
  C->cols[i] = nnz;
  C->nnz = nnz;
}

void dot(const FilteredBitmap& A,
         const BitmapBlock& B,
         FilteredBitmapBlock *C) {
  Size i, nnz = 0;
  for (i = 0; i < B.nnz; ++i) {
    C->cols[i] = nnz;

    auto b_row = B.rows[i];
    auto a_col = b_row % (A.blocks[0]->cols.size() - 1);
    auto block = b_row / (A.blocks[0]->cols.size() - 1);

    if (A.blocks[block]->cols[a_col+1] > A.blocks[block]->cols[a_col]) {
      C->rows[nnz] = A.blocks[block]->rows[ A.blocks[block]->cols[a_col] ];
      ++nnz;
    }
  }
  C->cols[i] = nnz;
  C->nnz = nnz;
}

}  // namespace engine
