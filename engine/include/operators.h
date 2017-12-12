/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 */
#ifndef ENGINE_INCLUDE_OPERATORS_H_
#define ENGINE_INCLUDE_OPERATORS_H_

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 1024
#endif

#include <google/protobuf/arena.h>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "src/block.pb.h"
#include "src/label-block.pb.h"
#include "src/database.pb.h"

namespace engine {

void krao(block* A, block* B, block* C,
          int rows_A = 1, bool CSC = false , bool move = false);

// In is either a block* (filter measure) or a label_block* (filter dimension)
void filter(const block& in,
            block* out,
            bool(*f)(float));

void filter(const label_block& in,
            block* out,
            bool(*f)(std::string));

void map(std::vector<block*> in,
         block* out,
         auto(*f)(auto));

bool loadDatabase(database* db,
                  const std::string& data_path,
                  const std::string& database);

matrix* getMatrix(database* db,
                  const std::string& table,
                  const std::string& column);

bool is_fk(database* db,
           const std::string& table,
           const std::string& column);

bool loadBlock(matrix* m,
               google::protobuf::Arena *arena,
               int idx);

bool loadLabelBlock(matrix* m,
               google::protobuf::Arena *arena,
               int idx);

void createDatabase(database* db,
                    const std::string& data_path,
                    const std::string& database);

void createTable(database* db,
                 const std::string& table,
                 const std::map<std::string, matrix_Type>& columns,
                 const std::string& pk,
                 const std::map<std::string,
                                std::tuple<std::string, std::string>>& fks,
                 google::protobuf::Arena *arena);

void insertValue(matrix* m, auto value);

/*bool importCSV(matrix* m,
               const std::string& file_path,
               const std::string& database_path,
               const std::string& table,
               const std::string& column);
*/
// csv import measure
// csv import dimension
// csv import fk


bool storeMatrix(matrix* m);
bool storeBlock(matrix* m, int idx);

}  // namespace engine

#endif  // ENGINE_INCLUDE_OPERATORS_H_
