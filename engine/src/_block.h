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
  block(const std::vector<float>& v,
        const std::vector<int>& r,
        const std::vector<int>& c);
  // virtual ~block();
  inline bool operator==(const block& b) {
    return values == b.values &&
           rows == b.rows &&
           columns == b.columns;
  }

  // private:
  std::vector<float> values;
  std::vector<int> rows;
  std::vector<int> columns;
};  // class block
}  // namespace engine

#endif  // ENGINE_SRC_BLOCK_H_
