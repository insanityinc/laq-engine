/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for LAQ parser
 */
#include <gtest/gtest.h>
#include "include/laq-driver.h"

TEST(laq_parser, q6) {
  laq::driver d;
  ASSERT_FALSE(d.parse("queries/laq/6.laq"));
  ASSERT_EQ(d.getQuery(),
            "a=filter(args[0]>=\"1994-01-01\"&&args[0]<=\"1995-01-01\")\n"
            "b=filter(args[0]>=0.05&&args[0]<=0.07)\n"
            "c=hadamard(a,b)\n"
            "d=map(args[0]*args[1])\n"
            "e=filter(args[0]<24)\n"
            "f=hadamard(d,e)\n"
            "g=hadamard(c,f)\n"
            "h=sum(g)\n"
            "return(h)\n");
}

int
main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
