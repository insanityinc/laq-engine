/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_OPERATORS_H_
#define ENGINE_INCLUDE_OPERATORS_H_
#include "src/block.pb.h"

namespace engine {

void krao(block* A, block* B, block* C,
          int rows_A = 1, bool CSC = false , bool move = false);

}  // namespace engine

#endif  // ENGINE_INCLUDE_OPERATORS_H_
