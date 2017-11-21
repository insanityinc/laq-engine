/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_MATRIX_H_
#define ENGINE_INCLUDE_MATRIX_H_
#include <map>
#include <string>
#include "src/block.h"

namespace engine {

class matrix {
 public:
  matrix(const std::string& path);
  bool init();
  bool load()

 private:
  std::string path;
  int nnz;
  int nrows;
  int ncols;

  std::map<int, block> blocks;

  /* friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      ar & nnz;
      ar & ncols;
      ar & nrows;
  }
  */
}  // class matrix
}  // namespace engine

#endif  // ENGINE_INCLUDE_MATRIX_H_
