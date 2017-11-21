/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for Protocol Buffer file read/write product
 */
#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/arena.h>
#include "include/operators.h"

TEST(krao, fbv_fbv) {
  engine::block res;
  krao(fbv, fbv, &res);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, *fbv));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
