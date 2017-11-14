/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef ENGINE_SRC_BLOCK_H_
#define ENGINE_SRC_BLOCK_H_
#include <vector>
// #include "matrix.h"

namespace engine {

class block {
 public:
  block();
  virtual ~block();

  // private:
  int index;
  std::vector<int> rows;
  std::vector<int> columns;
  std::vector<float> values;
};  // class block
}  // namespace engine

#endif  // ENGINE_SRC_BLOCK_H_
