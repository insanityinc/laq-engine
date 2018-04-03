/*
 * Copyright (c) 2018 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_BLOCK_H_
#define ENGINE_INCLUDE_BLOCK_H_

#include <fstream>
#include <string>
#include <vector>
#include "include/types.hpp"

namespace engine {

struct Block {
  Size nnz;

  template <class Archive>
  void serialize( Archive & ar ) {
    ar(nnz);
  }
};

struct LabelBlock : public Block {
  std::vector<Literal> labels;

  LabelBlock();

  void load(std::string filePath);
  void save(std::string filePath);

  void insert(Literal label);
};

struct DecimalVectorBlock : public Block {
  std::vector<Decimal> values;

  DecimalVectorBlock();

  void load(std::string filePath);
  void save(std::string filePath);

  void insert(Decimal value);
};

struct BitmapBlock : public Block {
  std::vector<MultiPrecision> rows;

  BitmapBlock();

  void load(std::string filePath);
  void save(std::string filePath);

  void insert(MultiPrecision row);
};

struct FilteredBitVectorBlock : public Block {
  std::vector<MultiPrecision> cols;

  FilteredBitVectorBlock();
};

struct DecimalMapBlock : public Block {
  std::vector<Decimal> values;
  std::vector<MultiPrecision> rows;

  DecimalMapBlock();
};

struct FilteredDecimalVectorBlock : public Block {
  std::vector<Decimal> values;
  std::vector<MultiPrecision> cols;

  FilteredDecimalVectorBlock();
};

struct FilteredBitmapBlock : public Block {
  std::vector<MultiPrecision> rows;
  std::vector<MultiPrecision> cols;

  FilteredBitmapBlock();
};

struct FilteredDecimalMapBlock : public Block {
  std::vector<Decimal> values;
  std::vector<MultiPrecision> rows;
  std::vector<MultiPrecision> cols;

  FilteredDecimalMapBlock();
};

}  // namespace engine

#endif  // ENGINE_INCLUDE_BLOCK_H_
