#include <gtest/gtest.h>
#include "LogicFunction.h"
#include "ZhegalkinPolynomial.h"

using namespace std;


TEST(ZhegalkinTest, AND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    ZhegalkinPolynomial poly(tt);
    string result = poly.getPolynomial();
    EXPECT_EQ(result, "a&b");
}

TEST(ZhegalkinTest, OR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    ZhegalkinPolynomial poly(tt);
    string result = poly.getPolynomial();
    EXPECT_TRUE(result == "b + a + a&b" || result == "a + b + a&b");
}

TEST(ZhegalkinTest, NOT) {
    LogicFunction logic("!a");
    const TruthTable& tt = logic.getTruthTable();
    ZhegalkinPolynomial poly(tt);
    string result = poly.getPolynomial();
    EXPECT_EQ(result, "1 + a");
}

TEST(ZhegalkinTest, XOR) {
    LogicFunction logic("a~b");
    const TruthTable& tt = logic.getTruthTable();
    ZhegalkinPolynomial poly(tt);
    string result = poly.getPolynomial();
    EXPECT_TRUE(result == "1 + a + b" || result == "1 + b + a");
}

TEST(ZhegalkinTest, Implication) {
    LogicFunction logic("a->b");
    const TruthTable& tt = logic.getTruthTable();
    ZhegalkinPolynomial poly(tt);
    string result = poly.getPolynomial();
    EXPECT_TRUE(result == "1 + a + a&b" || result == "1 + a&b + a");
}

TEST(ZhegalkinTest, ThreeVar) {
    LogicFunction logic("a->(b->c)");
    const TruthTable& tt = logic.getTruthTable();
    ZhegalkinPolynomial poly(tt);
    string result = poly.getPolynomial();
    EXPECT_TRUE(result == "1 + a&b + a&b&c" || result == "1 + a&b&c + a&b");
}

TEST(ZhegalkinTest, AllMethods) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    ZhegalkinPolynomial poly(tt);

    string result = poly.getPolynomial();
    EXPECT_EQ(result, "a&b");

    LogicFunction logic2("a|b");
    const TruthTable& tt2 = logic2.getTruthTable();
    ZhegalkinPolynomial poly2(tt2);
    result = poly2.getPolynomial();
    EXPECT_TRUE(result == "b + a + a&b" || result == "a + b + a&b");

    LogicFunction logic3("!a");
    const TruthTable& tt3 = logic3.getTruthTable();
    ZhegalkinPolynomial poly3(tt3);
    result = poly3.getPolynomial();
    EXPECT_EQ(result, "1 + a");
}
