/*
 * Copyright (c) 2017 João Afonso. All rights Reserved.
 */
#include "include/operators.h"

namespace engine {

// Replace all this functions with a single template
void
filter(const block& in,
       block* out,
       bool(*f)(std::vector<float>)) {
  for (int i = 0; i < in.values_size(); ++i) {
    std::vector<float> v = {in.values(i)};
    if ((*f)(v))
      out->add_columns(i);
  }
}

void
filter(const label_block& in,
       block* out,
       bool(*f)(std::vector<std::string>)) {
  for (int i = 0; i < in.values_size(); ++i) {
    std::vector<std::string> v = {in.values(i)};
    if ((*f)(v))
      out->add_columns(i);
  }
}

}  // namespace engine
