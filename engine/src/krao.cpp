/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */
#include "include/krao.hpp"

namespace engine {

void krao(const FilteredBitVectorBlock& A,
          const DecimalVectorBlock& B,
          FilteredDecimalVectorBlock *C) {
  Size i, nnz = 0;
  for (i = 0; i < A.cols.size() - 1; ++i) {
    if ( A.cols[i+1] > A.cols[i] ) {
      C->values[nnz] = B.values[i];
      ++nnz;
    }
  }
  C->cols = A.cols;
}

void krao(const FilteredBitVectorBlock& A,
          const FilteredBitVectorBlock& B,
          FilteredBitVectorBlock *C) {
  Size i, nnz = 0;

  // std::cout << "A_nnz: " << A.nnz << ", B_nnz: " << B.nnz << ", C_nnz: " << C->nnz << std::endl;

  // std::cout << "A_cols: " << A.cols.size() << ", B_cols: " << B.cols.size() << ", C_cols: " << C->cols.size() << std::endl;

  if (A.nnz < B.nnz) {
    for (i = 0; i < A.cols.size() - 1; ++i) {
      C->cols[i] = nnz;
      if ( (A.cols[i+1] > A.cols[i]) && (B.cols[i+1] > B.cols[i]) ) {
        ++nnz;
      }
    }
  } else {
    for (i = 0; i < A.cols.size() - 1; ++i) {
      C->cols[i] = nnz;
      if ( (B.cols[i+1] > B.cols[i]) && (A.cols[i+1] > A.cols[i]) ) {
        ++nnz;
      }
    }
  }

  C->cols[i] = nnz;
  C->nnz = nnz;
}

}  // namespace engine
