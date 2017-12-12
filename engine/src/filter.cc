/*
 * Copyright (c) 2017 João Afonso. All rights Reserved.
 */
#include "include/operators.h"

namespace engine {

void filter(const block& in,
            block* out,
            bool(*f)(float)) {
  for (int i = 0; i < in.values_size(); ++i) {
    if ((*f)(in.values(i)))
      out->add_columns(i);
  }
}

void filter(const label_block& in,
            block* out,
            bool(*f)(std::string)) {
  for (int i = 0; i < in.values_size(); ++i) {
    if ((*f)(in.values(i)))
      out->add_columns(i);
  }
}

}  // namespace engine
