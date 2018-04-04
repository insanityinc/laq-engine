/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_TYPES_H_
#define ENGINE_INCLUDE_TYPES_H_

#include <string>

// TODO(João): Use a cross platform lib to have coherent basic types
// Use GMP or other to represent multi precision
typedef double Decimal;
typedef uint64_t MultiPrecision;
typedef std::string Literal;
typedef size_t Size;

#ifndef BSIZE
#define BSIZE 1024
#endif

#endif  // ENGINE_INCLUDE_TYPES_H_
