/*
 * Copyright (c) 2017 João Afonso. All rights reserved.
 *
 * Unit tests for LAQ parser
 */
#include <gtest/gtest.h>
#include "include/database.hpp"
#include "include/laq_driver.hpp"

TEST(laqParser, q6) {
  laq::driver d;
  ASSERT_FALSE(d.parse("queries/laq/6.laq"));
  ASSERT_EQ(d.getQuery(),
            "a=filter(args[0]>=\"1994-01-01\"&&args[0]<\"1995-01-01\")\n"
            "b=filter(args[0]>=0.05&&args[0]<=0.07)\n"
            "c=filter(args[0]<24)\n"
            "d=hadamard(a,b)\n"
            "e=hadamard(c,d)\n"
            "f=lift(args[0]*args[1])\n"
            "g=hadamard(e,f)\n"
            "h=sum(g)\n"
            "return(h)\n");
}

TEST(toCpp, q6) {
  laq::driver d;
  engine::Database db("data/laq", "TPCH_1");
  ASSERT_FALSE(d.parse("queries/laq/6.laq"));

  // When complete read the cpp file
  ASSERT_EQ(d.toCpp(db),
            "/*\n"
            " * Copyright (c) 2018 João Afonso. All rights reserved.\n"
            " *\n"
            " * This query was automatically generated\n"
            " */\n"
            "#include <string>\n"
            "#include <vector>\n"
            "#include \"include/block.hpp\"\n"
            "#include \"include/database.hpp\"\n"
            "#include \"include/dot.hpp\"\n"
            "#include \"include/filter.hpp\"\n"
            "#include \"include/fold.hpp\"\n"
            "#include \"include/krao.hpp\"\n"
            "#include \"include/lift.hpp\"\n"
            "#include \"include/matrix.hpp\"\n"
            "#include \"include/types.hpp\"\n"
            "int main() {\n"
            "  engine::Database db(\n"
            "    \"data/laq\",\n"
            "    \"TPCH_1\");\n\n"
            "  return 0;\n"
            "}\n")
;}
int
main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
