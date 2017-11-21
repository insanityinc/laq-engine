/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for Khatri-Rao product
 */
#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/arena.h>
#include "include/operators.h"

engine::block *fbv, *bm, *fbm, *dv, *fdv, *dm, *fdm;

// Test COO without move

// 0x11 (fbv-fbv)
TEST(krao, fbv_fbv) {
  engine::block res;
  krao(fbv, fbv, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fbv));
}

// 0x12 (fbv-bm)
TEST(krao, fbv_bm) {
  engine::block res;
  krao(fbv, bm, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fbm));
}

// 0x13 (fbv-fbm)
TEST(krao, fbv_fbm) {
  engine::block res;
  krao(fbv, fbm, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fbm));
}

// 0x14 (fbv-dv)
TEST(krao, fbv_dv) {
  engine::block res;
  krao(fbv, dv, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fdv));
}

// 0x15 (fbv-fdv)
TEST(krao, fbv_fdv) {
  engine::block res;
  krao(fbv, fdv, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fdv));
}

// 0x16 (fbv-dm)
TEST(krao, fbv_dm) {
  engine::block res;
  krao(fbv, dm, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fdm));
}

// 0x17 (fbv-fdm)
TEST(krao, fbv_fdm) {
  engine::block res;
  krao(fbv, fdm, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fdm));
}

int main(int argc, char **argv) {
  std::vector<float> values = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
  std::vector<int> rows = {0, 2, 3, 4, 2, 1, 3, 0, 1, 1};

  std::vector<float> fvalues = {0.1, 0.3, 0.4, 0.6, 0.7, 0.9};
  std::vector<int> frows = {2, 4, 2, 3, 0, 1};
  std::vector<int> fcolumns = {1, 3, 4, 6, 7, 9};

  google::protobuf::Arena arena;

  fbv = google::protobuf::Arena::CreateMessage<engine::block>(&arena);
  for (auto& c : fcolumns)
    fbv->add_columns(c);

  bm = google::protobuf::Arena::CreateMessage<engine::block>(&arena);
  for (auto& r : rows)
    bm->add_rows(r);

  fbm = google::protobuf::Arena::CreateMessage<engine::block>(&arena);
  for (unsigned int i = 0; i < frows.size(); ++i) {
    fbm->add_rows(frows[i]);
    fbm->add_columns(fcolumns[i]);
  }

  dv = google::protobuf::Arena::CreateMessage<engine::block>(&arena);
  for (auto& v : values)
    dv->add_values(v);

  fdv = google::protobuf::Arena::CreateMessage<engine::block>(&arena);
  for (unsigned int i = 0; i < fvalues.size(); ++i) {
    fdv->add_values(fvalues[i]);
    fdv->add_columns(fcolumns[i]);
  }

  dm = google::protobuf::Arena::CreateMessage<engine::block>(&arena);
  for (unsigned int i = 0; i < values.size(); ++i) {
    dm->add_values(values[i]);
    dm->add_rows(rows[i]);
  }

  fdm = google::protobuf::Arena::CreateMessage<engine::block>(&arena);
  for (unsigned int i = 0; i < fvalues.size(); ++i) {
    fdm->add_values(fvalues[i]);
    fdm->add_rows(frows[i]);
    fdm->add_columns(fcolumns[i]);
  }

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
