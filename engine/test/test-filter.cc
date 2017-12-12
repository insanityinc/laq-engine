/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for Khatri-Rao product
 */
#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/arena.h>
#include "include/operators.h"

bool f_measure(auto arg) {
  return arg > 2;
}

bool f_dimension(auto arg) {
  return arg == "B";
}

TEST(filter, measure) {
  engine::block measure, res, out;

  std::vector<float> values = {0, 3, 2, 0, 5, 4, 1, 2, 9.5, 0};
  for (auto& v : values)
    measure.add_values(v);

  std::vector<int> fcolumns = {1, 4, 5, 8};
  for (auto& c : fcolumns)
    out.add_columns(c);

  filter(measure, &res, &f_measure);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, out));
}

TEST(filter, dimension) {
  engine::label_block dimension;
  engine::block res, out;

  std::vector<std::string> values =
    {"A", "B", "C", "C", "B", "B", "A", "C", "B", "A"};
  for (auto& v : values)
    dimension.add_values(v);

  std::vector<int> fcolumns = {1, 4, 5, 8};
  for (auto& c : fcolumns)
    out.add_columns(c);

  filter(dimension, &res, f_dimension);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, out));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
