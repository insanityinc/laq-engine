/*
 * Copyright (c) 2018 João Afonso. All rights Reserved.
 */

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include "include/block.hpp"

namespace engine {

LabelBlock::LabelBlock() : Block(), labels(BSIZE, "") {}

void LabelBlock::load(std::string filePath) {
  std::ifstream ifs(filePath, std::ios::in | std::ios::binary);
  if (ifs.is_open()) {
    {
      cereal::BinaryInputArchive iarchive(ifs);
      iarchive(cereal::base_class<Block>(this), labels);
    }
    ifs.close();
  }
}

void LabelBlock::save(std::string filePath) {
  std::ofstream ofs(filePath, std::ios::out | std::ios::binary);
  if (ofs.is_open()) {
    {
      cereal::BinaryOutputArchive oarchive(ofs);
      oarchive(CEREAL_NVP(cereal::base_class<Block>(this)), CEREAL_NVP(labels));
    }
    ofs.close();
  }
}

// make inline!
void LabelBlock::insert(Literal label) {
  labels[nnz++] = label;
}

DecimalVectorBlock::DecimalVectorBlock() : Block(), values(BSIZE, 0) {}

void DecimalVectorBlock::load(std::string filePath) {
  std::ifstream ifs(filePath, std::ios::in | std::ios::binary);
  if (ifs.is_open()) {
    {
      cereal::BinaryInputArchive iarchive(ifs);
      iarchive(cereal::base_class<Block>(this), values);
    }
    ifs.close();
  }
}

void DecimalVectorBlock::save(std::string filePath) {
  std::ofstream ofs(filePath, std::ios::out | std::ios::binary);
  if (ofs.is_open()) {
    {
      cereal::BinaryOutputArchive oarchive(ofs);
      oarchive(cereal::base_class<Block>(this), values);
    }
    ofs.close();
  }
}

// make inline!
void DecimalVectorBlock::insert(Decimal value) {
  values[nnz++] = value;
}

BitmapBlock::BitmapBlock() : Block(), rows(BSIZE, 0) {}

void BitmapBlock::load(std::string filePath) {
  std::ifstream ifs(filePath, std::ios::in | std::ios::binary);
  if (ifs.is_open()) {
    {
      cereal::BinaryInputArchive iarchive(ifs);
      iarchive(cereal::base_class<Block>(this), rows);
    }
    ifs.close();
  }
}

void BitmapBlock::save(std::string filePath) {
  std::ofstream ofs(filePath, std::ios::out | std::ios::binary);
  if (ofs.is_open()) {
    {
      cereal::BinaryOutputArchive oarchive(ofs);
      oarchive(cereal::base_class<Block>(this), rows);
    }
    ofs.close();
  }
}

// make inline!
void BitmapBlock::insert(MultiPrecision row) {
  rows[nnz++] = row;
}

FilteredBitVectorBlock::FilteredBitVectorBlock() : Block(), cols(BSIZE + 1, 0) {
  cols[0] = 0;
}

DecimalMapBlock::DecimalMapBlock() : Block(), values(BSIZE, 0), rows(BSIZE, 0) {}

FilteredDecimalVectorBlock::FilteredDecimalVectorBlock() : Block(), values(BSIZE, 0), cols(BSIZE + 1, 0) {
  cols[0] = 0;
}

FilteredBitmapBlock::FilteredBitmapBlock() : Block(), rows(BSIZE, 0), cols(BSIZE + 1, 0) {
  cols[0] = 0;
}

FilteredDecimalMapBlock::FilteredDecimalMapBlock() : Block(), values(BSIZE, 0), rows(BSIZE, 0), cols(BSIZE + 1, 0) {
  cols[0] = 0;
}

}  // namespace engine
