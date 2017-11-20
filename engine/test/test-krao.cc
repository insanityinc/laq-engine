/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for Khatri-Rao product
 */
#include <gtest/gtest.h>
#include "include/operators.h"

engine::block *fbv, *bm, *fbm, *dv, *fdv, *dm, *fdm;

// Test COO without move

// 0x11 (fbv-fbv)
TEST(krao, fbv_fbv) {
  engine::block res;
  res.index = 0;
  krao(fbv, fbv, &res);
  ASSERT_TRUE(res == *fbv);
}

// 0x12 (fbv-bm)
TEST(krao, fbv_bm) {
  engine::block res;
  res.index = 0;
  krao(fbv, bm, &res);
  ASSERT_TRUE(res == *fbm);
}

// 0x13 (fbv-fbm)
TEST(krao, fbv_fbm) {
  engine::block res;
  res.index = 0;
  krao(fbv, fbm, &res);
  ASSERT_TRUE(res == *fbm);
}

// 0x14 (fbv-dv)
TEST(krao, fbv_dv) {
  engine::block res;
  res.index = 0;
  krao(fbv, dv, &res);
  ASSERT_TRUE(res == *fdv);
}

// 0x15 (fbv-fdv)
TEST(krao, fbv_fdv) {
  engine::block res;
  res.index = 0;
  krao(fbv, fdv, &res);
  ASSERT_TRUE(res == *fdv);
}

// 0x16 (fbv-dm)
TEST(krao, fbv_dm) {
  engine::block res;
  res.index = 0;
  krao(fbv, dm, &res);
  ASSERT_TRUE(res == *fdm);
}

// 0x17 (fbv-fdm)
TEST(krao, fbv_fdm) {
  engine::block res;
  res.index = 0;
  krao(fbv, fdm, &res);
  ASSERT_TRUE(res == *fdm);
}

int main(int argc, char **argv) {
  std::vector<float> _f = std::vector<float>();
  std::vector<int> _i = std::vector<int>();
  std::vector<float> values = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
  std::vector<int> rows = {0, 2, 3, 4, 2, 1, 3, 0, 1, 1};

  std::vector<float> fvalues = {0.1, 0.3, 0.4, 0.6, 0.7, 0.9};
  std::vector<int> frows = {2, 4, 2, 3, 0, 1};
  std::vector<int> fcolumns = {1, 3, 4, 6, 7, 9};

  fbv = new engine::block(0, _f, _i, fcolumns);
  bm = new engine::block(0, _f, rows, _i);
  fbm = new engine::block(0, _f, frows, fcolumns);
  dv = new engine::block(0, values, _i, _i);
  fdv = new engine::block(0, fvalues, _i, fcolumns);
  dm = new engine::block(0, values, rows, _i);
  fdm = new engine::block(0, fvalues, frows, fcolumns);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
