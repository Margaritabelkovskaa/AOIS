#include <gtest/gtest.h>
#include "LogicFunction.h"
#include "Minimization.h"

using namespace std;


TEST(MinimizationTest, CalculationMethodAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("a & b") != string::npos);
}

TEST(MinimizationTest, CalculationMethodOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("a | b") != string::npos || output.find("b | a") != string::npos);
}

TEST(MinimizationTest, CalculationMethodImplication) {
    LogicFunction logic("a->b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("!a | b") != string::npos || output.find("b | !a") != string::npos);
}

TEST(MinimizationTest, CalculationMethodXOR) {
    LogicFunction logic("a~b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("!a & !b") != string::npos || output.find("a & b") != string::npos);
}

TEST(MinimizationTest, CalculationMethodThreeVar) {
    LogicFunction logic("a->(b->c)");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("c | !b | !a") != string::npos);
}

TEST(MinimizationTest, CalculationMethodAllFunctions) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b") != string::npos);

    LogicFunction logic2("a|b");
    const TruthTable& tt2 = logic2.getTruthTable();
    Minimization min2(tt2);

    testing::internal::CaptureStdout();
    min2.calculateMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a | b") != string::npos || output.find("b | a") != string::npos);

    LogicFunction logic3("!a");
    const TruthTable& tt3 = logic3.getTruthTable();
    Minimization min3(tt3);

    testing::internal::CaptureStdout();
    min3.calculateMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("!a") != string::npos);
}

TEST(MinimizationTest, CalculationTableMethodAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculationTableMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Таблица покрытия") != string::npos);
    EXPECT_TRUE(output.find("a & b") != string::npos);
}

TEST(MinimizationTest, CalculationTableMethodOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculationTableMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("m") != string::npos);
    EXPECT_TRUE(output.find("X") != string::npos);
}

TEST(MinimizationTest, CalculationTableMethodAllFunctions) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculationTableMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Таблица покрытия") != string::npos);
    EXPECT_TRUE(output.find("a & b") != string::npos);

    LogicFunction logic2("a|b");
    const TruthTable& tt2 = logic2.getTruthTable();
    Minimization min2(tt2);

    testing::internal::CaptureStdout();
    min2.calculationTableMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("m") != string::npos);
    EXPECT_TRUE(output.find("X") != string::npos);

    LogicFunction logic3("a->(b->c)");
    const TruthTable& tt3 = logic3.getTruthTable();
    Minimization min3(tt3);

    testing::internal::CaptureStdout();
    min3.calculationTableMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("c") != string::npos);
    EXPECT_TRUE(output.find("!b") != string::npos);
    EXPECT_TRUE(output.find("!a") != string::npos);
}

TEST(MinimizationTest, KarnaughMethodAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Карта Карно") != string::npos);
    EXPECT_TRUE(output.find("a & b") != string::npos);
}

TEST(MinimizationTest, KarnaughMethodOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("a | b") != string::npos || output.find("b | a") != string::npos);
}

TEST(MinimizationTest, KarnaughMethodThreeVar) {
    LogicFunction logic("a->(b->c)");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    string output = testing::internal::GetCapturedStdout();

    // Проверяем наличие карты Карно в выводе
    EXPECT_TRUE(output.find("Карта Карно") != string::npos || 
                output.find("Karnaugh map") != string::npos);
    
    // Проверяем результат минимизации (должен быть c | !b | !a или эквивалент)
    EXPECT_TRUE(output.find("c | !b | !a") != string::npos ||
                output.find("!a | !b | c") != string::npos ||
                output.find("!b | c | !a") != string::npos);
}

TEST(MinimizationTest, KarnaughMethodFourVar) {
    LogicFunction logic("(a->b)~(c->d)");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Карта Карно") != string::npos || 
                output.find("Karnaugh map") != string::npos);
    
    // Проверяем, что результат содержит хотя бы одну из импликант
    EXPECT_TRUE(output.find("b & d") != string::npos || 
                output.find("!a & !c") != string::npos ||
                output.find("a & c") != string::npos ||
                output.find("!b & !d") != string::npos);
}

TEST(MinimizationTest, KarnaughMethodAllFunctions) {
    // Тест для AND
    {
        SCOPED_TRACE("Testing AND function a&b");
        LogicFunction logic("a&b");
        const TruthTable& tt = logic.getTruthTable();
        Minimization min(tt);

        testing::internal::CaptureStdout();
        min.karnaughMethod();
        string output = testing::internal::GetCapturedStdout();
        
        EXPECT_TRUE(output.find("Карта Карно") != string::npos || 
                    output.find("Karnaugh map") != string::npos);
        EXPECT_TRUE(output.find("a & b") != string::npos);
    }

    // Тест для OR
    {
        SCOPED_TRACE("Testing OR function a|b");
        LogicFunction logic("a|b");
        const TruthTable& tt = logic.getTruthTable();
        Minimization min(tt);

        testing::internal::CaptureStdout();
        min.karnaughMethod();
        string output = testing::internal::GetCapturedStdout();
        
        EXPECT_TRUE(output.find("Карта Карно") != string::npos || 
                    output.find("Karnaugh map") != string::npos);
        // Для OR результат может быть "a | b" или "b | a"
        EXPECT_TRUE(output.find("a | b") != string::npos ||
                    output.find("b | a") != string::npos);
    }

    // Тест для трех переменных
    {
        SCOPED_TRACE("Testing implication a->(b->c)");
        LogicFunction logic("a->(b->c)");
        const TruthTable& tt = logic.getTruthTable();
        Minimization min(tt);

        testing::internal::CaptureStdout();
        min.karnaughMethod();
        string output = testing::internal::GetCapturedStdout();
        
        EXPECT_TRUE(output.find("Карта Карно") != string::npos || 
                    output.find("Karnaugh map") != string::npos);
        EXPECT_TRUE(output.find("c | !b | !a") != string::npos ||
                    output.find("!a | !b | c") != string::npos);
    }
}

TEST(MinimizationTest, SingleMinterm) {
    LogicFunction logic("a&b&c");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b & c") != string::npos);

    testing::internal::CaptureStdout();
    min.calculationTableMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b & c") != string::npos);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b & c") != string::npos);
}

TEST(MinimizationTest, AllOnes2Var) {
    LogicFunction logic("a|!a");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("1") != string::npos);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("1") != string::npos);
}

TEST(MinimizationTest, AllZeros2Var) {
    LogicFunction logic("a&!a");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("0") != string::npos);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("0") != string::npos);
}

TEST(MinimizationTest, AllOnes3Var) {
    LogicFunction logic("a|!a|b|!b|c|!c");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("1") != string::npos);
}

TEST(MinimizationTest, AllZeros3Var) {
    LogicFunction logic("a&!a&b&!b&c&!c");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("0") != string::npos);
}

TEST(MinimizationTest, Complex4VarFunction) {
    LogicFunction logic("(a&b)|(c&d)");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b") != string::npos || output.find("c & d") != string::npos);

    testing::internal::CaptureStdout();
    min.calculationTableMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b") != string::npos || output.find("c & d") != string::npos);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a & b") != string::npos || output.find("c & d") != string::npos);
}

TEST(MinimizationTest, FiveVarFunction) {
    LogicFunction logic("(a&b)|(c&d)|e");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.karnaughMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Карта Карно") != string::npos || 
                output.find("Karnaugh map") != string::npos);
}

TEST(MinimizationTest, RedundantImplicants) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Удаление лишних импликант") != string::npos ||
                output.find("Removing redundant implicants") != string::npos);
}

TEST(MinimizationTest, PrimeImplicantsAnd) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    vector<Term> prime = min.getPrimeImplicants();
    EXPECT_FALSE(prime.empty());
}

TEST(MinimizationTest, PrimeImplicantsOr) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    vector<Term> prime = min.getPrimeImplicants();
    EXPECT_EQ(prime.size(), 2);
}

TEST(MinimizationTest, PrimeImplicantsXor) {
    LogicFunction logic("a~b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    vector<Term> prime = min.getPrimeImplicants();
    EXPECT_EQ(prime.size(), 2);
}

TEST(MinimizationTest, GluingCorrectness) {
    LogicFunction logic("(a&b)|(a&!b)|(!a&b)");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("a | b") != string::npos || output.find("b | a") != string::npos);
}

TEST(MinimizationTest, EmptyMinterms) {
    LogicFunction logic("a&!a");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("0") != string::npos);

    testing::internal::CaptureStdout();
    min.calculationTableMethod();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("0") != string::npos);
}

TEST(MinimizationTest, ConstantOne) {
    LogicFunction logic("a|!a");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    testing::internal::CaptureStdout();
    min.calculateMethod();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("1") != string::npos);
}

TEST(MinimizationTest, GlueTerms) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    vector<Term> prime = min.getPrimeImplicants();
    EXPECT_FALSE(prime.empty());
}

TEST(MinimizationTest, GetPrimeImplicants) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    Minimization min(tt);

    vector<Term> prime = min.getPrimeImplicants();
    EXPECT_EQ(prime.size(), 2);
    
    const auto& vars = logic.getTruthTable().getVariables();
    string term1 = prime[0].toString(vars);
    string term2 = prime[1].toString(vars);
    
    EXPECT_TRUE((term1 == "a" && term2 == "b") || (term1 == "b" && term2 == "a"));
}
