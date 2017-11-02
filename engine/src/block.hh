/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef ENGINE_SRC_BLOCK_HH_
#define ENGINE_SRC_BLOCK_HH_
#include "matrix.hh"

namespace engine {

class block {
 public:
  block();
  virtual ~block();

 private:
  int index;
  std::vector<int> rows;
  std::vector<int> columns;
  std::vector<float> values;

}  // class block
}  // namespace engine

#endif  // ENGINE_SRC_BLOCK_HH_
