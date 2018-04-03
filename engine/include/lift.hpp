/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_LIFT_H_
#define ENGINE_INCLUDE_LIFT_H_

#include <vector>
#include "include/block.hpp"

namespace engine {

void lift(Decimal(*f)(std::vector<Decimal>),
          const std::vector<DecimalVectorBlock>& in,
          DecimalVectorBlock* out);

// function must be inline, needs modification?
void lift(Decimal(*f)(std::vector<Decimal>),
          const std::vector<DecimalVectorBlock>& in,
          const FilteredBitVectorBlock& iter,
          FilteredDecimalVectorBlock* out);

}  // namespace engine

#endif  // ENGINE_INCLUDE_LIFT_H_
