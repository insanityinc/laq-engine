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
    "\n"
    "inline bool filter_a(std::vector<engine::Literal> args){\n"
    "  return args[0]>=\"1994-01-01\"&&args[0]<\"1995-01-01\";\n"
    "}\n"
    "inline bool filter_b(std::vector<engine::Decimal> args){\n"
    "  return args[0]>=0.05&&args[0]<=0.07;\n"
    "}\n"
    "inline bool filter_d(std::vector<engine::Decimal> args){\n"
    "  return args[0]<24;\n"
    "}\n"
    "inline engine::Decimal lift_f(std::vector<engine::Decimal> args) {\n"
    "  return args[0]*args[1];\n"
    "}\n"
    "\n"
    "int main() {\n"
    "  engine::Database db(\n"
    "    \"data/la\",\n"
    "    \"TPCH_1\",\n"
    "    false);\n"
    "\n"
    "  engine::DecimalVector *lineitem_discount =\n"
    "    new engine::DecimalVector(db.data_path,\n"
    "      db.database_name,\n"
    "      \"lineitem\",\n"
    "      \"discount\");\n"
    "  engine::DecimalVector *lineitem_extendedprice =\n"
    "    new engine::DecimalVector(db.data_path,\n"
    "      db.database_name,\n"
    "      \"lineitem\",\n"
    "      \"extendedprice\");\n"
    "  engine::DecimalVector *lineitem_quantity =\n"
    "    new engine::DecimalVector(db.data_path,\n"
    "      db.database_name,\n"
    "      \"lineitem\",\n"
    "      \"quantity\");\n"
    "  engine::Bitmap *lineitem_shipdate =\n"
    "    new engine::Bitmap(db.data_path,\n"
    "      db.database_name,\n"
    "      \"lineitem\",\n"
    "      \"shipdate\");\n"
    "\n"
    "  engine::FilteredBitVector *a_pred =\n"
    "    new engine::FilteredBitVector(lineitem_shipdate->nLabelBlocks);\n"
    "  engine::FilteredBitVector *a =\n"
    "    new engine::FilteredBitVector(lineitem_shipdate->nBlocks);\n"
    "  engine::FilteredBitVector *b =\n"
    "    new engine::FilteredBitVector(lineitem_discount->nBlocks);\n"
    "  engine::FilteredBitVector *c =\n"
    "    new engine::FilteredBitVector(a->nBlocks);\n"
    "  engine::FilteredBitVector *d =\n"
    "    new engine::FilteredBitVector(lineitem_quantity->nBlocks);\n"
    "  engine::FilteredBitVector *e =\n"
    "    new engine::FilteredBitVector(c->nBlocks);\n"
    "  engine::DecimalVector *f =\n"
    "    new engine::DecimalVector(lineitem_extendedprice->nBlocks);\n"
    "  engine::FilteredDecimalVector *g =\n"
    "    new engine::FilteredDecimalVector(e->nBlocks);\n"
    "  engine::Decimal *h =\n"
    "    new engine::Decimal();\n"
    "\n"
    "  return 0;\n"
    "}\n"
  );
}

int
main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
