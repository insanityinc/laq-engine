/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_FOLD_H_
#define ENGINE_INCLUDE_FOLD_H_

#include <vector>
#include "include/block.hpp"

namespace engine {

void sum(const FilteredDecimalVectorBlock& in,
         Decimal *acc);

}  // namespace engine

#endif  // ENGINE_INCLUDE_FOLD_H_
