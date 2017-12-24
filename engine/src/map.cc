/*
 * Copyright (c) 2017 João Afonso. All rights Reserved.
 */
#include "include/operators.h"

namespace engine {

void
map(const std::vector<block*>& in,
    block* out,
    float(*f)(std::vector<float>)) {
  int size = in[0]->values_size();

  out->mutable_values()->Reserve(size);
  std::vector<float> v(in.size());

  for (int i = 0; i < size; ++i) {
    for (size_t j = 0; j < in.size(); ++j)
      v[j] = in[j]->values(i);
    out->add_values((*f)(v));
  }
}

}  // namespace engine
