/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */
#include "include/fold.hpp"

namespace engine {

void sum(const FilteredDecimalVectorBlock& in,
         Decimal *acc) {
  for (auto& v : in.values) {
    (*acc) += v;
  }
}

}  // namespace engine
