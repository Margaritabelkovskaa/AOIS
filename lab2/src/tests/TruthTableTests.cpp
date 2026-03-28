#include <gtest/gtest.h>
#include "TruthTable.h"
#include "ExpressionParser.h"

using namespace std;

TEST(TruthTableTest, ConstructorAND) {
    ExpressionParser parser("a&b");
    TruthTable tt(parser);

    EXPECT_EQ(tt.getVariables().size(), 2);
    EXPECT_EQ(tt.getTable().size(), 4);
    EXPECT_EQ(tt.getTrueMinterms().size(), 1);
    EXPECT_EQ(tt.getFalseMinterms().size(), 3);
}

TEST(TruthTableTest, ConstructorOR) {
    ExpressionParser parser("a|b");
    TruthTable tt(parser);

    EXPECT_EQ(tt.getTrueMinterms().size(), 3);
    EXPECT_EQ(tt.getFalseMinterms().size(), 1);
}

TEST(TruthTableTest, GetMintermString) {
    ExpressionParser parser("a&b");
    TruthTable tt(parser);

    EXPECT_EQ(tt.getMintermString(3), "a & b");
}

TEST(TruthTableTest, GetMaxtermString) {
    ExpressionParser parser("a&b");
    TruthTable tt(parser);

    EXPECT_EQ(tt.getMaxtermString(0), "(a | b)");
    EXPECT_EQ(tt.getMaxtermString(1), "(a | !b)");
    EXPECT_EQ(tt.getMaxtermString(2), "(!a | b)");
}

TEST(TruthTableTest, Print) {
    ExpressionParser parser("a&b");
    TruthTable tt(parser);

    testing::internal::CaptureStdout();
    tt.print();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("a b | f") != string::npos);
    EXPECT_TRUE(output.find("0 0 | 0") != string::npos);
    EXPECT_TRUE(output.find("1 1 | 1") != string::npos);
}

TEST(TruthTableTest, AllMethods) {
    ExpressionParser parser("a&b");
    TruthTable tt(parser);

    EXPECT_EQ(tt.getMintermString(0), "!a & !b");
    EXPECT_EQ(tt.getMintermString(1), "!a & b");
    EXPECT_EQ(tt.getMintermString(2), "a & !b");
    EXPECT_EQ(tt.getMintermString(3), "a & b");

    EXPECT_EQ(tt.getMaxtermString(0), "(a | b)");
    EXPECT_EQ(tt.getMaxtermString(1), "(a | !b)");
    EXPECT_EQ(tt.getMaxtermString(2), "(!a | b)");
    EXPECT_EQ(tt.getMaxtermString(3), "(!a | !b)");

    const auto& table = tt.getTable();
    EXPECT_EQ(table.size(), 4);
    EXPECT_EQ(table[0].size(), 3);

    const auto& vars = tt.getVariables();
    EXPECT_EQ(vars.size(), 2);
    EXPECT_EQ(vars[0], 'a');
    EXPECT_EQ(vars[1], 'b');

    const auto& true_min = tt.getTrueMinterms();
    const auto& false_min = tt.getFalseMinterms();
    EXPECT_EQ(true_min.size(), 1);
    EXPECT_EQ(false_min.size(), 3);
}
