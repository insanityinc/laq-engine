/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for Khatri-Rao product
 */
#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/arena.h>
#include "include/operators.h"

auto f_map(std::vector<auto> args) {
  return args[0] + args[1] * 10;
}

bool f_measure(std::vector<auto> args) {
  return args[0] > 2;
}

bool f_dimension(std::vector<auto> args) {
  return args[0] == "B";
}

TEST(map, simple) {
  engine::block arg0, arg1, res, out;

  std::vector<float> a0 = {1, 7, 3, 2.1, 8, 1, 3, 3.5, 1.2, 1};
  for (auto& v : a0)
    arg0.add_values(v);

  std::vector<float> a1 = {0, 3, 2, 0, 5, 4, 1, 2, 0.5, 0};
  for (auto& v : a1)
    arg1.add_values(v);

  std::vector<float> op = {1, 37, 23, 2.1, 58, 41, 13, 23.5, 6.2, 1};
  for (auto& c : op)
    out.add_values(c);

  std::vector<engine::block*> in = {&arg0, &arg1};
  map(in, &res, f_map);
  ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(res, out));
}

TEST(filter, measure) {
  engine::block measure, res, out;

  std::vector<float> values = {0, 3, 2, 0, 5, 4, 1, 2, 9.5, 0};
  for (auto& v : values)
    measure.add_values(v);

  std::vector<int> fcolumns = {1, 4, 5, 8};
  for (auto& c : fcolumns)
    out.add_columns(c);

  filter(measure, &res, f_measure);
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
