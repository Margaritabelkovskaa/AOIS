#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include "LogicFunction.h"
#include "Term.h"
#include "SKNFMinimization.h"
#include "TruthTable.h"
#include "ExpressionParser.h"

using namespace std;

TEST(SKNFMinimizationTest, Constructor) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    EXPECT_TRUE(true);
}

TEST(SKNFMinimizationTest, MinimizeAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    // Должно быть: (a | b) & (a | !b) & (!a | b)
    EXPECT_TRUE(output.find("a | b") != string::npos);
    EXPECT_TRUE(output.find("a | !b") != string::npos);
    EXPECT_TRUE(output.find("!a | b") != string::npos);
}

// Тест 3: Функция OR (a|b) - СКНФ: (a|b)
TEST(SKNFMinimizationTest, MinimizeOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    // Должно быть: (a | b)
    EXPECT_TRUE(output.find("a | b") != string::npos);
}

// Тест 4: Функция NOT (!a) - СКНФ: (a)
TEST(SKNFMinimizationTest, MinimizeNOT) {
    LogicFunction logic("!a");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    // Должно быть: (a)
    EXPECT_TRUE(output.find("a") != string::npos);
    EXPECT_TRUE(output.find("|") == string::npos); // нет ИЛИ
}

// Тест 5: Функция Implication (a->b) - СКНФ: (!a | b)
TEST(SKNFMinimizationTest, MinimizeImplication) {
    LogicFunction logic("a->b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("!a | b") != string::npos);
}

// Тест 6: Функция XOR (a~b) - СКНФ: (a|b) & (!a|!b)
TEST(SKNFMinimizationTest, MinimizeXOR) {
    LogicFunction logic("a~b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("a | b") != string::npos);
    EXPECT_TRUE(output.find("!a | !b") != string::npos);
}

// Тест 7: Функция a->(b->c) - СКНФ: (!a | !b | c)
TEST(SKNFMinimizationTest, MinimizeThreeVar) {
    LogicFunction logic("a->(b->c)");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("!a | !b | c") != string::npos ||
                output.find("c | !b | !a") != string::npos);
}

// Тест 8: Функция (a->b)~(c->d) - СКНФ: (a | b | !c | d) & (!a | b | c | d) & ...
TEST(SKNFMinimizationTest, MinimizeFourVar) {
    LogicFunction logic("(a->b)~(c->d)");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("a | b | !c | d") != string::npos);
}

// Тест 9: Константа 1 - СКНФ: 1 (пусто)
TEST(SKNFMinimizationTest, ConstantOne) {
    LogicFunction logic("a|!a");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("1") != string::npos);
}

// Тест 10: Константа 0 - СКНФ: 0
TEST(SKNFMinimizationTest, ConstantZero) {
    LogicFunction logic("a&!a");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeQuine();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("0") != string::npos);
}

// ==================== ТЕСТЫ ДЛЯ МЕТОДА ТАБЛИЦЫ ПОКРЫТИЯ ====================

TEST(SKNFMinimizationTest, TableMethodAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeTable();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Coverage table") != string::npos);
    EXPECT_TRUE(output.find("a | b") != string::npos);
}

TEST(SKNFMinimizationTest, TableMethodOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeTable();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("a | b") != string::npos);
}

TEST(SKNFMinimizationTest, TableMethodThreeVar) {
    LogicFunction logic("a->(b->c)");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeTable();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("!a | !b | c") != string::npos ||
                output.find("c | !b | !a") != string::npos);
}

// ==================== ТЕСТЫ ДЛЯ КАРТЫ КАРНО ====================

TEST(SKNFMinimizationTest, KarnaughMethodAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeKarnaugh();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Karnaugh map") != string::npos);
    EXPECT_TRUE(output.find("0") != string::npos); // нули на карте
}

TEST(SKNFMinimizationTest, KarnaughMethodOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeKarnaugh();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Karnaugh map") != string::npos);
}

TEST(SKNFMinimizationTest, KarnaughMethodThreeVar) {
    LogicFunction logic("a->(b->c)");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeKarnaugh();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Karnaugh map") != string::npos);
    EXPECT_TRUE(output.find("0") != string::npos); // нули на карте
}

// ==================== ТЕСТЫ ДЛЯ МЕТОДА MINIMIZEALL ====================

TEST(SKNFMinimizationTest, MinimizeAllAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeAll();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("SKNF Minimization") != string::npos);
    EXPECT_TRUE(output.find("SKNF Calculation-table method") != string::npos);
    EXPECT_TRUE(output.find("SKNF Karnaugh map minimization") != string::npos);
}

TEST(SKNFMinimizationTest, MinimizeAllOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    testing::internal::CaptureStdout();
    sknf.minimizeAll();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("a | b") != string::npos);
}

// ==================== ТЕСТЫ ДЛЯ ВСПОМОГАТЕЛЬНЫХ МЕТОДОВ ====================

TEST(SKNFMinimizationTest, GrayCode) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    EXPECT_EQ(sknf.grayCode(0), 0);
    EXPECT_EQ(sknf.grayCode(1), 1);
    EXPECT_EQ(sknf.grayCode(2), 3);
    EXPECT_EQ(sknf.grayCode(3), 2);
}

TEST(SKNFMinimizationTest, GrayToBinary) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    EXPECT_EQ(sknf.grayToBinary(0), 0);
    EXPECT_EQ(sknf.grayToBinary(1), 1);
    EXPECT_EQ(sknf.grayToBinary(3), 2);
    EXPECT_EQ(sknf.grayToBinary(2), 3);
}

TEST(SKNFMinimizationTest, GetMaxtermValues) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    vector<int> vals = sknf.getMaxtermValues(0);
    EXPECT_EQ(vals.size(), 2);
    EXPECT_EQ(vals[0], 0);
    EXPECT_EQ(vals[1], 0);
}

TEST(SKNFMinimizationTest, MaxtermToString) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    vector<int> vals = {0, 1};
    string str = sknf.maxtermToString(vals);
    EXPECT_TRUE(str.find("a") != string::npos);
    EXPECT_TRUE(str.find("!b") != string::npos);
}

// ==================== ТЕСТЫ ДЛЯ СКЛЕИВАНИЯ ====================

TEST(SKNFMinimizationTest, TryGlueMaxterm) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    Term t1(2);
    Term t2(2);
    Term result(2);
    
    t1.values = {0, 1};
    t2.values = {0, 0};
    
    EXPECT_TRUE(sknf.tryGlueMaxterm(t1, t2, result));
    EXPECT_EQ(result.values[0], 0);
    EXPECT_EQ(result.values[1], -1);
}

TEST(SKNFMinimizationTest, AddUnique) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    SKNFMinimization sknf(tt);
    
    vector<Term> list;
    Term t(2);
    t.values = {0, 1};
    
    sknf.addUnique(list, t);
    EXPECT_EQ(list.size(), 1);
    
    sknf.addUnique(list, t);
    EXPECT_EQ(list.size(), 1);
}

