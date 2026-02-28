#include <gtest/gtest.h>
#include "BinaryCode.h"

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
