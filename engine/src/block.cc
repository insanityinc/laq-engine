/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "src/block.h"

namespace engine {

block::block(const int idx) {
  index = idx;
  values = std::vector<float>();
  rows = std::vector<int>();
  columns = std::vector<int>();
}

block::block(const int idx,
             const std::vector<float>& v,
             const std::vector<int>& r,
             const std::vector<int>& c)
    : index(idx), values(v), rows(r), columns(c) {}



}  // namespace engine
