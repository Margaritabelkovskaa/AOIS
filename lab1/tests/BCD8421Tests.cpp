#include <gtest/gtest.h>
#include "BCD8421.h"

class BCD8421Test : public ::testing::Test {
protected:
    void SetUp() override {}
};

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

TEST_F(BCD8421Test, Zero) {
    BCD8421 bcd;
    bcd.fromDecimal(0);
    EXPECT_EQ(bcd.toDecimal(), 0);

    // Печать нуля
    testing::internal::CaptureStdout();
    bcd.printBCD();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

TEST_F(BCD8421Test, PrintBCD) {
    BCD8421 bcd;
    bcd.fromDecimal(12345);

    testing::internal::CaptureStdout();
    bcd.printBCD();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("12345") != std::string::npos);
}

TEST_F(BCD8421Test, AdditionalCode) {
    BCD8421 bcd;
    bcd.fromDecimal(-42); // должно автоматически перевести в доп. код

    // Проверяем что обратно получается правильно
    EXPECT_EQ(bcd.toDecimal(), -42);
}
