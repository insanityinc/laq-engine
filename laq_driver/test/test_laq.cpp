/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for LAQ parser
 */
#include <gtest/gtest.h>
#include "include/database.hpp"
#include "include/laq_driver.hpp"

#include <iostream>

TEST(laqParser, q6) {
  laq::driver d;
  ASSERT_FALSE(d.parse("queries/laq/6.laq"));
  ASSERT_EQ(d.getQuery(),
      "a=filter(args[0]>=\"1994-01-01\"&&args[0]<\"1995-01-01\")\n"
      "b=filter(args[0]>=0.05&&args[0]<=0.07)\n"
      "c=hadamard(a,b)\n"
      "d=filter(args[0]<24)\n"
      "e=hadamard(c,d)\n"
      "f=lift(args[0]*args[1])\n"
      "g=hadamard(e,f)\n"
      "h=sum(g)\n"
      "return(h)\n");
}

TEST(toCpp, q6) {
  laq::driver d;
  engine::Database db("data/la", "TPCH_1", false);
  ASSERT_TRUE(db.load());
  ASSERT_FALSE(d.parse("queries/laq/6.laq"));

  std::cout << d.toCpp(db) << std::endl;

  std::ifstream ifs("queries/cpp/6.cpp");
  std::string q6( (std::istreambuf_iterator<char>(ifs) ),
                  (std::istreambuf_iterator<char>()    ) );

  ASSERT_EQ(d.toCpp(db), q6);
}


TEST(toCpp, q14) {
  laq::driver d;
  engine::Database db("data/la", "TPCH_1", false);
  ASSERT_TRUE(db.load());
  ASSERT_FALSE(d.parse("queries/laq/14.laq"));

  std::ifstream ifs("queries/cpp/14.cpp");
  std::string q14( (std::istreambuf_iterator<char>(ifs) ),
                  (std::istreambuf_iterator<char>()    ) );
  ASSERT_EQ(d.toCpp(db), q14);
}

TEST(toCpp, q3) {
  laq::driver d;
  engine::Database db("data/la", "TPCH_1", false);
  ASSERT_TRUE(db.load());
  ASSERT_FALSE(d.parse("queries/laq/3.laq"));

  std::ifstream ifs("queries/cpp/3.cpp");
  std::string q3( (std::istreambuf_iterator<char>(ifs) ),
                  (std::istreambuf_iterator<char>()    ) );
  ASSERT_EQ(d.toCpp(db), q3);
}


int
main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
