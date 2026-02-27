#include <gtest/gtest.h>
#include <cmath>
#include <limits>

#include "BCD8421.h"
#include "BinaryCode.h"
#include "FloatIEEE754.h"

// ==================== ТЕСТЫ ДЛЯ КЛАССА BinaryCode ====================

class BinaryCodeTest : public ::testing::Test {
protected:
    void SetUp() override {}

    bool compareBinary(const BinaryCode& a, const BinaryCode& b) {
        for (int i = 0; i < 32; i++) {
            if (a.getBit(i) != b.getBit(i)) return false;
        }
        return true;
    }
};

TEST_F(BinaryCodeTest, ConstructorAndBasicMethods) {
    BinaryCode b;

    for (int i = 0; i < 32; i++) {
        EXPECT_EQ(b.getBit(i), 0);
    }

    b.setBit(0, 1);
    EXPECT_EQ(b.getBit(0), 1);
    EXPECT_EQ(b.getBit(1), 0);

    b.setBit(31, 1);
    EXPECT_EQ(b.getBit(31), 1);

    b.setBit(15, 0);
    EXPECT_EQ(b.getBit(15), 0);

    EXPECT_EQ(b.getBit(-1), 0);
    EXPECT_EQ(b.getBit(32), 0);
}

TEST_F(BinaryCodeTest, CopyFrom) {
    BinaryCode b1;
    b1.setBit(5, 1);
    b1.setBit(10, 1);
    b1.setBit(20, 1);

    BinaryCode b2;
    b2.copyFrom(b1);

    for (int i = 0; i < 32; i++) {
        EXPECT_EQ(b2.getBit(i), b1.getBit(i));
    }
}

TEST_F(BinaryCodeTest, FromDecimalToDirect) {
    struct TestCase {
        int input;
        int expectedSign;
    };

    TestCase tests[] = {
        {42, 0},
        {7, 0},
        {1, 0},
        {0, 0},
        {-42, 1},
    };

    for (const auto& test : tests) {
        BinaryCode b;
        b.fromDecimalToDirect(test.input);
        EXPECT_EQ(b.getBit(31), test.expectedSign);
        EXPECT_EQ(b.fromDirectToDecimal(), test.input);
    }
}

TEST_F(BinaryCodeTest, FromDirectToDecimal) {
    int testNumbers[] = { 0, 1, 42, 127, 255, 1024, 2147483647, -1, -42, -127, -2147483647 };

    for (int num : testNumbers) {
        BinaryCode b;
        b.fromDecimalToDirect(num);
        EXPECT_EQ(b.fromDirectToDecimal(), num);
    }
}

TEST_F(BinaryCodeTest, ReverseCode) {
    BinaryCode pos;
    pos.fromDecimalToDirect(42);
    BinaryCode posCopy;
    posCopy.copyFrom(pos);
    pos.toReverseCode();
    EXPECT_TRUE(compareBinary(pos, posCopy));

    BinaryCode neg;
    neg.fromDecimalToDirect(-42);
    neg.toReverseCode();

    EXPECT_EQ(neg.getBit(31), 1);

    BinaryCode expected;
    expected.fromDecimalToDirect(42);
    for (int i = 0; i < 31; i++) {
        EXPECT_EQ(neg.getBit(i), 1 - expected.getBit(i));
    }
}

TEST_F(BinaryCodeTest, AdditionalCode) {
    BinaryCode pos;
    pos.fromDecimalToDirect(42);
    BinaryCode posCopy;
    posCopy.copyFrom(pos);
    pos.toAdditionalCode();
    EXPECT_TRUE(compareBinary(pos, posCopy));

    int negNumbers[] = { -1, -42, -100, -127, -255 };
    for (int num : negNumbers) {
        BinaryCode b;
        b.fromDecimalToDirect(num);
        b.toAdditionalCode();

        BinaryCode back;
        back.copyFrom(b);
        back.fromAdditionalToDirect();
        EXPECT_EQ(back.fromDirectToDecimal(), num);
    }
}

TEST_F(BinaryCodeTest, BinaryAdd) {
    BinaryCode a, b;

    a.fromDecimalToDirect(5);
    b.fromDecimalToDirect(3);

    a.setBit(31, 0);
    b.setBit(31, 0);

    BinaryCode sum = BinaryCode::binaryAdd(a, b);

    EXPECT_EQ(sum.getBit(0), 0);
    EXPECT_EQ(sum.getBit(1), 0);
    EXPECT_EQ(sum.getBit(2), 0);
    EXPECT_EQ(sum.getBit(3), 1);
}

TEST_F(BinaryCodeTest, BinarySubtract) {
    BinaryCode a, b;

    a.fromDecimalToDirect(10);
    b.fromDecimalToDirect(4);

    a.setBit(31, 0);
    b.setBit(31, 0);

    BinaryCode diff = BinaryCode::binarySubtract(a, b);

    EXPECT_EQ(diff.fromDirectToDecimal(), 6);
}

TEST_F(BinaryCodeTest, IsGreaterOrEqual) {
    BinaryCode a, b;

    a.fromDecimalToDirect(10);
    b.fromDecimalToDirect(5);
    a.setBit(31, 0);
    b.setBit(31, 0);

    EXPECT_TRUE(BinaryCode::isGreaterOrEqual(a, b));
    EXPECT_FALSE(BinaryCode::isGreaterOrEqual(b, a));

    a.fromDecimalToDirect(5);
    b.fromDecimalToDirect(5);
    a.setBit(31, 0);
    b.setBit(31, 0);

    EXPECT_TRUE(BinaryCode::isGreaterOrEqual(a, b));
}

TEST_F(BinaryCodeTest, ShiftOperations) {
    BinaryCode b;
    b.fromDecimalToDirect(5);
    b.setBit(31, 0);

    b.shiftLeft();
    EXPECT_EQ(b.getBit(0), 0);
    EXPECT_EQ(b.getBit(1), 1);
    EXPECT_EQ(b.getBit(2), 0);
    EXPECT_EQ(b.getBit(3), 1);

    b.shiftRight();
    EXPECT_EQ(b.fromDirectToDecimal(), 5);
}

TEST_F(BinaryCodeTest, MultiplyDirect) {
    struct TestCase {
        int x;
        int y;
        int expected;
    };

    TestCase tests[] = {
        {7, 6, 42},
        {5, 5, 25},
        {12, 3, 36},
        {8, 7, 56},
        {0, 42, 0},
        {1, 1, 1},
        {-7, 6, -42},
        {7, -6, -42},
        {-7, -6, 42},
    };

    for (const auto& test : tests) {
        BinaryCode a, b;
        a.fromDecimalToDirect(test.x);
        b.fromDecimalToDirect(test.y);

        BinaryCode product = BinaryCode::multiplyDirect(a, b);
        int result = product.fromDirectToDecimal();

        EXPECT_EQ(result, test.expected)
            << "Failed: " << test.x << " * " << test.y;
    }
}

TEST_F(BinaryCodeTest, DivideRestoring) {
    struct TestCase {
        int x;
        int y;
        int expectedInt;
        double expectedFull;
    };

    TestCase tests[] = {
        {42, 7, 6, 6.0},
        {10, 3, 3, 3.3333333333},
        {100, 4, 25, 25.0},
        {7, 2, 3, 3.5},
        {1, 3, 0, 0.3333333333},
        {-42, 7, -6, -6.0},
        {42, -7, -6, -6.0},
        {-42, -7, 6, 6.0},
        {0, 5, 0, 0.0}
    };

    for (const auto& test : tests) {
        BinaryCode a, b;
        a.fromDecimalToDirect(test.x);
        b.fromDecimalToDirect(test.y);

        double decimalResult;
        BinaryCode remainder;
        BinaryCode quotient = BinaryCode::divideRestoring(a, b, decimalResult, remainder);

        EXPECT_NEAR(decimalResult, test.expectedFull, 0.0001)
            << "Failed: " << test.x << " / " << test.y;
    }
}

TEST_F(BinaryCodeTest, AddAdditional) {
    struct TestCase {
        int x;
        int y;
        int expected;
    };

    TestCase tests[] = {
        {15, 27, 42},
        {100, 200, 300},
        {0, 5, 5},
        {1, 1, 2},
        {50, -30, 20},
        {30, -50, -20},
        {100, -100, 0},
        {-20, -15, -35}
    };

    for (const auto& test : tests) {
        BinaryCode a, b;
        a.fromDecimalToDirect(test.x);
        b.fromDecimalToDirect(test.y);
        a.toAdditionalCode();
        b.toAdditionalCode();

        BinaryCode sum = BinaryCode::addAdditional(a, b);
        BinaryCode result;
        result.copyFrom(sum);
        result.fromAdditionalToDirect();

        EXPECT_EQ(result.fromDirectToDecimal(), test.expected)
            << "Failed: " << test.x << " + " << test.y;
    }
}

TEST_F(BinaryCodeTest, SubtractAdditional) {
    struct TestCase {
        int x;
        int y;
        int expected;
    };

    TestCase tests[] = {
        {100, 30, 70},
        {30, 100, -70},
        {50, 50, 0},
        {10, -5, 15},
        {-10, 5, -15}
    };

    for (const auto& test : tests) {
        BinaryCode a, b;
        a.fromDecimalToDirect(test.x);
        b.fromDecimalToDirect(test.y);
        a.toAdditionalCode();
        b.toAdditionalCode();

        BinaryCode diff = BinaryCode::subtractAdditional(a, b);
        BinaryCode result;
        result.copyFrom(diff);
        result.fromAdditionalToDirect();

        EXPECT_EQ(result.fromDirectToDecimal(), test.expected)
            << "Failed: " << test.x << " - " << test.y;
    }
}

// ==================== ТЕСТЫ ДЛЯ КЛАССА FloatIEEE754 ====================

class FloatIEEE754Test : public ::testing::Test {
protected:
    void SetUp() override {}

    bool isApproxEqual(float a, float b, float eps = 0.0001f) {
        if (std::isnan(a) && std::isnan(b)) return true;
        if (std::isinf(a) && std::isinf(b)) return (a > 0) == (b > 0);
        return fabs(a - b) < eps;
    }
};

TEST_F(FloatIEEE754Test, FromDecimal) {
    float testNumbers[] = { 0.0f, 1.0f, -1.0f, 42.5f, -3.14f, 0.5f, 0.000123f, 123456.789f };

    for (float num : testNumbers) {
        FloatIEEE754 f;
        f.fromDecimal(num);
        float result = f.toDecimal();

        EXPECT_TRUE(isApproxEqual(result, num))
            << "Failed for number: " << num << " got: " << result;
    }
}

TEST_F(FloatIEEE754Test, GetSetComponents) {
    FloatIEEE754 f;
    f.setSign(1);
    f.setExponent(130);
    f.setMantissa(0x123456);

    EXPECT_EQ(f.getSign(), 1);
    EXPECT_EQ(f.getExponent(), 130);
    EXPECT_EQ(f.getMantissa(), 0x123456);
}

TEST_F(FloatIEEE754Test, Add) {
    struct TestCase {
        float x;
        float y;
        float expected;
    };

    TestCase tests[] = {
        {10.5f, 3.2f, 13.7f},
        {1.1f, 2.2f, 3.3f},
        {-5.5f, 2.5f, -3.0f},
        {0.1f, 0.2f, 0.3f},
        {4.4f, 1.1f, 5.5f}
    };

    for (const auto& test : tests) {
        FloatIEEE754 a, b;
        a.fromDecimal(test.x);
        b.fromDecimal(test.y);

        FloatIEEE754 sum = FloatIEEE754::add(a, b);
        float result = sum.toDecimal();

        EXPECT_TRUE(isApproxEqual(result, test.expected))
            << "Failed: " << test.x << " + " << test.y << " = " << result << " expected " << test.expected;
    }
}

TEST_F(FloatIEEE754Test, Subtract) {
    struct TestCase {
        float x;
        float y;
        float expected;
    };

    TestCase tests[] = {
        {10.5f, 3.2f, 7.3f},
        {5.5f, 2.5f, 3.0f},
        {2.5f, 5.5f, -3.0f},
        {4.4f, 1.1f, 3.3f}
    };

    for (const auto& test : tests) {
        FloatIEEE754 a, b;
        a.fromDecimal(test.x);
        b.fromDecimal(test.y);

        FloatIEEE754 diff = FloatIEEE754::subtract(a, b);
        float result = diff.toDecimal();

        EXPECT_TRUE(isApproxEqual(result, test.expected))
            << "Failed: " << test.x << " - " << test.y;
    }
}

TEST_F(FloatIEEE754Test, Multiply) {
    struct TestCase {
        float x;
        float y;
        float expected;
    };

    TestCase tests[] = {
        {2.5f, 4.0f, 10.0f},
        {1.5f, 3.0f, 4.5f},
        {-2.0f, 3.0f, -6.0f},
        {4.4f, 1.1f, 4.84f}
    };

    for (const auto& test : tests) {
        FloatIEEE754 a, b;
        a.fromDecimal(test.x);
        b.fromDecimal(test.y);

        FloatIEEE754 product = FloatIEEE754::multiply(a, b);
        float result = product.toDecimal();

        EXPECT_TRUE(isApproxEqual(result, test.expected, 0.01f))
            << "Failed: " << test.x << " * " << test.y;
    }
}

TEST_F(FloatIEEE754Test, Divide) {
    struct TestCase {
        float x;
        float y;
        float expected;
    };

    TestCase tests[] = {
        {10.0f, 2.0f, 5.0f},
        {7.5f, 2.5f, 3.0f},
        {5.0f, 2.0f, 2.5f},
        {4.4f, 1.1f, 4.0f}
    };

    for (const auto& test : tests) {
        FloatIEEE754 a, b;
        a.fromDecimal(test.x);
        b.fromDecimal(test.y);

        FloatIEEE754 quotient = FloatIEEE754::divide(a, b);
        float result = quotient.toDecimal();

        EXPECT_TRUE(isApproxEqual(result, test.expected, 0.01f))
            << "Failed: " << test.x << " / " << test.y;
    }
}

// ==================== ТЕСТЫ ДЛЯ КЛАССА BCD8421 ====================

class BCD8421Test : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(BCD8421Test, FromDecimal) {
    struct TestCase {
        int input;
        int expectedValue;
    };

    TestCase tests[] = {
        {0, 0},
        {5, 5},
        {42, 42},
        {95, 95},
        {123, 123},
        {99999999, 99999999},
        {-42, -42}
    };

    for (const auto& test : tests) {
        BCD8421 bcd;
        bcd.fromDecimal(test.input);
        EXPECT_EQ(bcd.toDecimal(), test.input);
    }
}

TEST_F(BCD8421Test, Add) {
    struct TestCase {
        int x;
        int y;
        int expected;
    };

    TestCase tests[] = {
        {5, 3, 8},
        {9, 1, 10},
        {95, 7, 102},
        {42, 58, 100},
        {99, 1, 100},
        {-5, 3, -2},
        {5, -3, 2},
        {-5, -3, -8}
    };

    for (const auto& test : tests) {
        BCD8421 a, b;
        a.fromDecimal(test.x);
        b.fromDecimal(test.y);

        BCD8421 sum = BCD8421::add(a, b);
        EXPECT_EQ(sum.toDecimal(), test.expected)
            << "Failed: " << test.x << " + " << test.y;
    }
}

// ==================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ====================

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(IntegrationTest, BinaryCodeFullCycle) {
    int testNum = -12345;

    BinaryCode b;
    b.fromDecimalToDirect(testNum);
    EXPECT_EQ(b.fromDirectToDecimal(), testNum);

    b.toAdditionalCode();
    BinaryCode back;
    back.copyFrom(b);
    back.fromAdditionalToDirect();
    EXPECT_EQ(back.fromDirectToDecimal(), testNum);
}

TEST_F(IntegrationTest, FloatSpecialCases) {
    FloatIEEE754 f;

    f.fromDecimal(std::numeric_limits<float>::quiet_NaN());
    float result = f.toDecimal();
    EXPECT_TRUE(std::isnan(result));

    f.fromDecimal(std::numeric_limits<float>::infinity());
    result = f.toDecimal();
    EXPECT_TRUE(std::isinf(result) && result > 0);

    f.fromDecimal(-std::numeric_limits<float>::infinity());
    result = f.toDecimal();
    EXPECT_TRUE(std::isinf(result) && result < 0);
}

TEST_F(IntegrationTest, FloatExtremes) {
    FloatIEEE754 f;

    f.fromDecimal(1e-30f);
    EXPECT_GT(f.toDecimal(), 0);

    f.fromDecimal(1e30f);
    EXPECT_GT(f.toDecimal(), 0);
}

TEST_F(IntegrationTest, ZeroHandling) {
    BinaryCode zero;
    zero.fromDecimalToDirect(0);

    for (int i = 0; i < 32; i++) {
        EXPECT_EQ(zero.getBit(i), 0);
    }

    zero.toAdditionalCode();
    for (int i = 0; i < 32; i++) {
        EXPECT_EQ(zero.getBit(i), 0);
    }

    BCD8421 bcdZero;
    bcdZero.fromDecimal(0);
    EXPECT_EQ(bcdZero.toDecimal(), 0);

    FloatIEEE754 floatZero;
    floatZero.fromDecimal(0.0f);
    EXPECT_EQ(floatZero.toDecimal(), 0.0f);
}

TEST_F(IntegrationTest, BCDMaxValue) {
    BCD8421 bcd;
    bcd.fromDecimal(99999999);
    EXPECT_EQ(bcd.toDecimal(), 99999999);
}

TEST_F(IntegrationTest, MultipleOperations) {
    BinaryCode a, b;
    a.fromDecimalToDirect(50);
    b.fromDecimalToDirect(30);

    a.toAdditionalCode();
    b.toAdditionalCode();

    BinaryCode sum = BinaryCode::addAdditional(a, b);
    BinaryCode sumBack;
    sumBack.copyFrom(sum);
    sumBack.fromAdditionalToDirect();
    EXPECT_EQ(sumBack.fromDirectToDecimal(), 80);

    BinaryCode c;
    c.fromDecimalToDirect(20);
    c.toAdditionalCode();

    BinaryCode diff = BinaryCode::subtractAdditional(sum, c);
    BinaryCode diffBack;
    diffBack.copyFrom(diff);
    diffBack.fromAdditionalToDirect();
    EXPECT_EQ(diffBack.fromDirectToDecimal(), 60);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}