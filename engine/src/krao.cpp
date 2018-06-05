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
  C->nnz = nnz;
}

void krao(const FilteredBitVectorBlock& A,
          const FilteredBitVectorBlock& B,
          FilteredBitVectorBlock *C) {
  Size i, nnz = 0;

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

void krao(const FilteredDecimalVectorBlock& A,
          const FilteredBitVectorBlock& B,
          FilteredDecimalVectorBlock* C) {
  Size i, nnz = 0;

  if (A.nnz < B.nnz) {
    for (i = 0; i < A.cols.size() - 1; ++i) {
      C->cols[i] = nnz;
      if ( (A.cols[i+1] > A.cols[i]) && (B.cols[i+1] > B.cols[i]) ) {
        C->values[nnz] = A.values[A.cols[i+1]];
        ++nnz;
      }
    }
  } else {
    for (i = 0; i < A.cols.size() - 1; ++i) {
      C->cols[i] = nnz;
      if ( (B.cols[i+1] > B.cols[i]) && (A.cols[i+1] > A.cols[i]) ) {
        C->values[nnz] = A.values[A.cols[i+1]];
        ++nnz;
      }
    }
  }

  C->cols[i] = nnz;
  C->nnz = nnz;
}

void krao(const FilteredBitVectorBlock& A,
          const BitmapBlock& B,
          FilteredBitmapBlock *C) {
  Size i, nnz = 0;
  for (i = 0; i < A.cols.size() - 1; ++i) {
    if ( A.cols[i+1] > A.cols[i] ) {
      C->rows[nnz] = B.rows[i];
      ++nnz;
    }
  }
  C->cols = A.cols;
  C-> nnz = nnz;
}

void krao(const FilteredBitmapBlock& A,
          const BitmapBlock& B,
          FilteredBitmapBlock *C,
          Size Bnrows) {
  Size i, nnz = 0;
  for (i = 0; i < A.cols.size() - 1; ++i) {
    if ( A.cols[i+1] > A.cols[i] ) {
      C->rows[nnz] = (A.rows[A.cols[i+1]] * Bnrows) + B.rows[i];
      ++nnz;
    }
  }
  C->cols = A.cols;
  C->nnz = nnz;
}

void krao(const BitmapBlock& A,
          const FilteredBitmapBlock& B,
          FilteredBitmapBlock *C,
          Size Bnrows) {
  Size i, nnz = 0;
  for (i = 0; i < B.cols.size() - 1; ++i) {
    if ( B.cols[i+1] > B.cols[i] ) {
      C->rows[nnz] = (A.rows[i] * Bnrows) + B.rows[B.cols[i+1]];
      ++nnz;
    }
  }
  C->cols = B.cols;
  C-> nnz = nnz;
}

void krao(const FilteredBitmapBlock& A,
          const FilteredBitVectorBlock& B,
          FilteredBitmapBlock* C) {
  Size i, nnz = 0;

  if (A.nnz < B.nnz) {
    for (i = 0; i < A.cols.size() - 1; ++i) {
      C->cols[i] = nnz;
      if ( (A.cols[i+1] > A.cols[i]) && (B.cols[i+1] > B.cols[i]) ) {
        C->rows[nnz] = A.rows[A.cols[i+1]];
        ++nnz;
      }
    }
  } else {
    for (i = 0; i < A.cols.size() - 1; ++i) {
      C->cols[i] = nnz;
      if ( (B.cols[i+1] > B.cols[i]) && (A.cols[i+1] > A.cols[i]) ) {
        C->rows[nnz] = A.rows[A.cols[i+1]];
        ++nnz;
      }
    }
  }

  C->cols[i] = nnz;
  C->nnz = nnz;
}

void krao(const FilteredBitmapBlock& A,
          const DecimalVectorBlock& B,
          FilteredDecimalMapBlock *C) {
  Size i, nnz = 0;
  for (i = 0; i < A.cols.size() - 1; ++i) {
    if ( A.cols[i+1] > A.cols[i] ) {
      C->values[nnz] = B.values[i];
      ++nnz;
    }
  }
  C->cols = A.cols;
  C->rows = A.rows;
  C->nnz = nnz;
}

}  // namespace engine
