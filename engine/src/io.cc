/*
 * Copyright (c) 2017 Hubble83 Afonso. All rights Reserved.
 */
#include "include/operators.h"

namespace engine {

bool
loadDatabase(database* db,
             const std::string& data_path,
             const std::string& database) {
  fstream input(data_path + "/" + database + "/meta.dat",
                ios::in | ios::binary);
  if (!input || !db->ParseFromIstream(&input)) {
    std::cerr << "Failed to parse database metadata." << std::endl;
    return false;
  }
  return true;
}

/*
  bool
  loadMatrix(database* db,
             matrix* m,
             const std::string& table,
             const std::string& column) {
    // TODO: Check column exists in database!!!
    m->set_table(table);
    m->set_column(column);
    fstream input(db->data_path() + "/"
                  + db->tbl_name() + "/"
                  + table + "/"
                  + column + "/meta.dat",
                  ios::in | ios::binary);
    if (!input || !m->ParseFromIstream(&input)) {
      std::cerr << "Failed to parse matrix metadata." << std::endl;
      return false;
    }
    return true;
  }
*/

matrix*
getMatrix(database* db,
          const std::string& table,
          const std::string& column) {
  return &(db->tables()[table].columns[column]);
}

bool
is_fk(database* db, const std::string& table, const std::string& column) {
    return db->tables().find(table) != db->tables().end() &&
      db->tables()[table].columns().find(column)
      != db->tables()[table].columns().end();
}

bool
loadBlock(database* db,
          matrix* m,
          google::protobuf::Arena *arena,
          int idx) {
  // TODO(Hubble83): Check column exists in db and idx is a valid block
  ostringstream str;
  str << db->data_path() << "/"
      << db->tbl_name() << "/"
      << m->table() << "/"
      << m->column() << "/blocks/"
      << idx << ".dat";
  std::string file_path = str.str();

  block* b = google::protobuf::Arena::CreateMessage<block>(arena);

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

bool
loadLabelBlock(database* db,
               matrix* m,
               google::protobuf::Arena *arena,
               int idx) {
  // TODO(Hubble83): Check column exists in db and idx is a valid label block
  ostringstream str;
  if ()
  str << db->data_path() << "/"
      << db->tbl_name() << "/"
      << m->table() << "/"
      << m->column() << "/blocks/"
      << idx << ".dat";
  std::string file_path = str.str();

  block* b = google::protobuf::Arena::CreateMessage<block>(arena);

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

void
createDatabase(database *db,
               const std::string& data_path,
               const std::string& database) {
  db->set_data_path(data_path);
  db->set_tbl_name(database);
}

void
createTable(database* db,
            const std::string& table,
            const std::map<std::string, matrix_Type>& columns,
            const std::string& pk,
            const std::map<std::string,
                           std::tuple<std::string, std::string>>& fks
            google::protobuf::Arena *arena) {
  table *new_table = google::protobuf::Arena::CreateMessage<table>(&arena);

  auto tbl_cols = new_table->mutable_columns();
  for (map<auto, auto>::iterator it = columns.begin();
      it != columns.end(); ++it) {
    matrix *m = google::protobuf::Arena::CreateMessage<matrix>(&arena);

    matrix->set_nnz(0);
    matrix->set_nrows(0);
    matrix->set_ncols(0);
    matrix->set_type(columns[it->first]);
    matrix->set_table(table);
    matrix->set_column(it->first);

    (*tbl_cols)[it->first] = *m;
  }

  new_table->set_pk(pk);

  auto tbl_fks = new_table->mutable_fks();
  for (map<auto, auto>::iterator it = fks.begin(); it != fks.end(); ++it) {
    label_path *lp = google::protobuf::Arena::CreateMessage<label_path>(&arena);

    lp->set_l_table(std::get<0>(fks[it->first]));
    lp->set_l_column(std::get<1>(fks[it->first]));

    (*tbl_fks)[it->first] = *lp;
  }

  auto tables = db->mutable_tables();
  (*tables)[table] = *new_table;
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
