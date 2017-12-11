/*
 * Copyright (c) 2017 João Afonso. All rights Reserved.
 */
#include "include/operators.h"

namespace engine {

bool
loadMatrix(matrix* m,
           const std::string& data_path,
           const std::string& database,
           const std::string& table,
           const std::string& column) {
  m->set_database(database);
  m->set_table(table);
  m->set_column(column);
  fstream input(database + "/" + table + "/" + column + "/meta.dat",
                ios::in | ios::binary);
  if (!input || !m->ParseFromIstream(&input)) {
    std::cerr << "Failed to parse matrix metadata." << std::endl;
    return false;
  }
  return true;
}

bool
loadBlock(matrix* m,
          const std::string& data_path,
          google::protobuf::Arena *arena,
          int idx) {
  ostringstream str;
  str << data_path << "/"
      << matrix->database() << "/"
      << matrix->table() << "/"
      << matrix->column() << "/blocks/"
      << idx << ".dat";
  std::string file_path = str.str();

  block *b = google::protobuf::Arena::CreateMessage<block>(arena);

  fstream input(file_path, ios::in | ios::binary);
  if (!input || !b->ParseFromIstream(&input)) {
    std::cerr << "Failed to parse block." << std::endl;
    return false;
  } else {
    auto map = m->mutable_blocks();
    (*map)[idx] = *b;
    return true;
  }
}

/*
  bool
  importCSV(matrix* m,
          google::protobuf::Arena *arena,
          const std::string& file_path,
          const std::string& database,
          const std::string& table,
          const std::string& column,
          const std::string& table_pk = ""
          const std::string& column_pk = "") {
    ifstream input(file_path);

    int nnz = 0;

    google::protobuf::Arena *arena,
    block *b = google::protobuf::Arena::CreateMessage<block>(arena);
    label_block *lb = google::protobuf::Arena::CreateMessage<label_block>(arena);
    std::unordered_map<std::string, int> reverse_labels;

    for (std::string line; std::getline(input, line); ++nnz) {
      // row is zero if unexistant; index start at 1
      int row = reverse_labels[line] - 1;
      if (row < 0) {
        row = nnz;
        reverse_labels[row] = nnz + 1;
        lb->add_value(line);
      }

      if (nnz % BLOCK_SIZE == 0) {
        // create file block[nnz / BLOCK_SIZE]
        ++block_count;
        i = 0;
      }
    }
  }
*/

}  // namespace engine
