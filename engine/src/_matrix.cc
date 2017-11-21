/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#include "include/matrix.h"

namespace engine {

matrix::matrix(const std::string& path) {

}

bool
matrix::init(matrix *m) {
  std::ifstream fmeta(m->path + "/metadata.dat");
  if (fmeta.good()) {
    // boost::archive::binary_iarchive ia(file, std::ios::binary);
    ia >> (*m);
    file.close();
    return true;
  }
  return false;
}

bool
matrix::load(matrix *m, )

}  // namespace engine
