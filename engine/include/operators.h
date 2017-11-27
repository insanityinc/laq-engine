/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_OPERATORS_H_
#define ENGINE_INCLUDE_OPERATORS_H_

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 1024
#endif

#include <string>
#include <google/protobuf/arena.h>
#include "src/block.pb.h"
#include "src/label-block.pb.h"
#include "src/matrix.pb.h"

namespace engine {

void krao(block* A, block* B, block* C,
          int rows_A = 1, bool CSC = false , bool move = false);

bool loadMatrix(matrix* m,
                const std::string& database_path,
                const std::string& table,
                const std::string& column);

bool loadBlock(matrix* m,
               google::protobuf::Arena *arena,
               int idx);

bool importCSV(matrix* m,
               const std::string& file_path,
               const std::string& database_path,
               const std::string& table,
               const std::string& column);

// csv import measure
// csv import dimension
// csv import fk


bool storeMatrix(matrix* m);
bool storeBlock(matrix* m, int idx);

}  // namespace engine

#endif  // ENGINE_INCLUDE_OPERATORS_H_
