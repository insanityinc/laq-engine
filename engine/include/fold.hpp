/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_FOLD_H_
#define ENGINE_INCLUDE_FOLD_H_

#include "include/block.hpp"
#include "include/matrix.hpp"

namespace engine {

void sum(const FilteredDecimalVectorBlock& in,
         Decimal *acc);

void sum(const FilteredDecimalMapBlock& in,
         FilteredDecimalMapAcc *acc);

}  // namespace engine

#endif  // ENGINE_INCLUDE_FOLD_H_
