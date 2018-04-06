/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_FILTER_H_
#define ENGINE_INCLUDE_FILTER_H_

#include <vector>
#include "include/block.hpp"
#include "include/types.hpp"

namespace engine {

void filter(bool(*f)(std::vector<Decimal>),
            const std::vector<DecimalVectorBlock>& in,
            FilteredBitVectorBlock* out);

void filter(bool(*f)(std::vector<Literal>),
            const std::vector<LabelBlock>& in,
            FilteredBitVectorBlock* out);

void filter(bool(*f)(std::vector<Decimal>),
            const std::vector<DecimalVectorBlock>& in,
            const FilteredBitVectorBlock& iter,
            FilteredBitVectorBlock* out);

}  // namespace engine

#endif  // ENGINE_INCLUDE_LIFT_H_
