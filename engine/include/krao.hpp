/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_KRAO_H_
#define ENGINE_INCLUDE_KRAO_H_

#include <vector>
#include "include/block.hpp"

namespace engine {

void krao(const FilteredBitVectorBlock& A,
          const DecimalVectorBlock& B,
          FilteredDecimalVectorBlock *C);

void krao(const FilteredBitVectorBlock& A,
          const FilteredBitVectorBlock& B,
          FilteredBitVectorBlock *C);

}  // namespace engine

#endif  // ENGINE_INCLUDE_KRAO_H_
