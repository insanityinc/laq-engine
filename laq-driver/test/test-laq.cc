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
            "a=filter(l_shipdate>=\"1994-01-01\"&&l_shipdate<=\"1995-01-01\")\n"
            "b=filter(l_discount>=0.05&&l_discount<=0.07)\n"
            "c=filter(l_quantity<24)\n"
            "d=hadamard(a,b)\n"
            "e=hadamard(c,d)\n"
            "f=map(l_extendedprice*l_discount)\n"
            "g=hadamard(e,f)\n"
            "h=sum(g)\n"
            "return(h)\n");
}

int
main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
