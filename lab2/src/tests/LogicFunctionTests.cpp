#include <gtest/gtest.h>
#include "LogicFunction.h"

using namespace std;

TEST(LogicFunctionTest, TruthTableAND) {
    LogicFunction logic("a&b");

    string index = logic.getIndexForm();
    EXPECT_EQ(index, "0001");

    string sdnf = logic.getSDNF();
    EXPECT_EQ(sdnf, "(a & b)");

    string sknf = logic.getSKNF();
    EXPECT_EQ(sknf, "(a | b) & (a | !b) & (!a | b)");
}

TEST(LogicFunctionTest, TruthTableOR) {
    LogicFunction logic("a|b");

    string index = logic.getIndexForm();
    EXPECT_EQ(index, "0111");

    string sdnf = logic.getSDNF();
    EXPECT_EQ(sdnf, "(!a & b) | (a & !b) | (a & b)");
}

TEST(LogicFunctionTest, TruthTableNOT) {
    LogicFunction logic("!a");

    string index = logic.getIndexForm();
    EXPECT_EQ(index, "10");

    string sdnf = logic.getSDNF();
    EXPECT_EQ(sdnf, "(!a)");
}

TEST(LogicFunctionTest, TruthTableImplication) {
    LogicFunction logic("a->b");

    string index = logic.getIndexForm();
    EXPECT_EQ(index, "1101");

    string sdnf = logic.getSDNF();
    EXPECT_EQ(sdnf, "(!a & !b) | (!a & b) | (a & b)");
}

TEST(LogicFunctionTest, TruthTableXOR) {
    LogicFunction logic("a~b");

    string index = logic.getIndexForm();
    EXPECT_EQ(index, "1001");

    string sdnf = logic.getSDNF();
    EXPECT_EQ(sdnf, "(!a & !b) | (a & b)");
}

TEST(LogicFunctionTest, NumericForms) {
    LogicFunction logic("a&b");

    string numeric_sdnf = logic.getNumericSDNF();
    EXPECT_EQ(numeric_sdnf, "СДНФ: 3");

    string numeric_sknf = logic.getNumericSKNF();
    EXPECT_EQ(numeric_sknf, "СКНФ: 0, 1, 2");
}

TEST(LogicFunctionTest, IndexForm) {
    LogicFunction logic("a&b");
    EXPECT_EQ(logic.getIndexForm(), "0001");

    LogicFunction logic2("a|b");
    EXPECT_EQ(logic2.getIndexForm(), "0111");
}

TEST(LogicFunctionTest, ThreeVarFunction) {
    LogicFunction logic("a->(b->c)");

    string index = logic.getIndexForm();
    EXPECT_EQ(index, "11111101");

    string numeric_sdnf = logic.getNumericSDNF();
    EXPECT_EQ(numeric_sdnf, "СДНФ: 0, 1, 2, 3, 4, 5, 7");
}

TEST(LogicFunctionTest, FourVarFunction) {
    LogicFunction logic("(a->b)~(c->d)");

    string index = logic.getIndexForm();
    EXPECT_EQ(index, "1101110100101101");
    EXPECT_EQ(index.size(), 16);
}

TEST(LogicFunctionTest, AllMethodsAND) {
    LogicFunction logic("a&b");

    testing::internal::CaptureStdout();
    logic.printTruthTable();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a b | f") != string::npos);

    testing::internal::CaptureStdout();
    logic.checkPostClasses();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("T0") != string::npos);

    string poly = logic.getZhegalkinPolynomial();
    EXPECT_EQ(poly, "a&b");

    testing::internal::CaptureStdout();
    logic.findDummyVariables();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Фиктивных переменных нет") != string::npos);

    testing::internal::CaptureStdout();
    logic.booleanDifferentiation();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("df/da:") != string::npos);

    testing::internal::CaptureStdout();
    logic.minimizeCalculation();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b") != string::npos);

    testing::internal::CaptureStdout();
    logic.minimizeCalculationTable();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Таблица покрытия") != string::npos);

    testing::internal::CaptureStdout();
    logic.minimizeKarnaugh();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Карта Карно") != string::npos);
}

TEST(LogicFunctionTest, AllMethodsOR) {
    LogicFunction logic("a|b");

    string poly = logic.getZhegalkinPolynomial();
    EXPECT_TRUE(poly == "b + a + a&b" || poly == "a + b + a&b");

    testing::internal::CaptureStdout();
    logic.minimizeCalculation();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a | b") != string::npos || output.find("b | a") != string::npos);
}

TEST(LogicFunctionTest, AllMethodsNOT) {
    LogicFunction logic("!a");

    string poly = logic.getZhegalkinPolynomial();
    EXPECT_EQ(poly, "1 + a");

    testing::internal::CaptureStdout();
    logic.minimizeCalculation();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("!a") != string::npos);
}

TEST(LogicFunctionTest, AllMethodsImplication) {
    LogicFunction logic("a->b");

    string poly = logic.getZhegalkinPolynomial();
    EXPECT_TRUE(poly == "1 + a + a&b" || poly == "1 + a&b + a");

    testing::internal::CaptureStdout();
    logic.minimizeCalculation();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("!a | b") != string::npos || output.find("b | !a") != string::npos);
}

TEST(LogicFunctionTest, AllMethodsXOR) {
    LogicFunction logic("a~b");

    string poly = logic.getZhegalkinPolynomial();
    EXPECT_TRUE(poly == "1 + a + b" || poly == "1 + b + a");

    testing::internal::CaptureStdout();
    logic.minimizeCalculation();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("!a & !b") != string::npos || output.find("a & b") != string::npos);
}

TEST(LogicFunctionTest, AllMethodsThreeVar) {
    LogicFunction logic("a->(b->c)");

    string poly = logic.getZhegalkinPolynomial();
    EXPECT_TRUE(poly == "1 + a&b + a&b&c" || poly == "1 + a&b&c + a&b");

    testing::internal::CaptureStdout();
    logic.minimizeCalculation();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("c | !b | !a") != string::npos);

    testing::internal::CaptureStdout();
    logic.minimizeKarnaugh();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("c | !b | !a") != string::npos);
}
