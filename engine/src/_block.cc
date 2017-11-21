/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "src/block.h"

namespace engine {

block::block() {
  values = std::vector<float>();
  rows = std::vector<int>();
  columns = std::vector<int>();
}

block::block(const std::vector<float>& v,
             const std::vector<int>& r,
             const std::vector<int>& c)
    : values(v), rows(r), columns(c) {}

}  // namespace engine
