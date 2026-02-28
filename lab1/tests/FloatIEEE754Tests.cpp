#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "FloatIEEE754.h"

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

TEST_F(FloatIEEE754Test, SpecialCases) {
    FloatIEEE754 f;

    // Денормализованные числа
    f.fromDecimal(1.4e-45f);
    EXPECT_GT(f.toDecimal(), 0);

    // Бесконечность
    f.fromDecimal(INFINITY);
    EXPECT_TRUE(std::isinf(f.toDecimal()));

    // Ноль
    f.fromDecimal(0.0f);
    EXPECT_EQ(f.toDecimal(), 0.0f);

    // Максимальное значение
    f.fromDecimal(3.4e38f);
    EXPECT_GT(f.toDecimal(), 0);

    // Минимальное положительное
    f.fromDecimal(1.18e-38f);
    EXPECT_GT(f.toDecimal(), 0);
}

TEST_F(FloatIEEE754Test, DivideByZero) {
    FloatIEEE754 a, b;
    a.fromDecimal(10.0f);
    b.fromDecimal(0.0f);

    FloatIEEE754 result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));
}

TEST_F(FloatIEEE754Test, ZeroDivideByZero) {
    FloatIEEE754 a, b;
    a.fromDecimal(0.0f);
    b.fromDecimal(0.0f);

    FloatIEEE754 result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));
}

TEST_F(FloatIEEE754Test, SpecialValues) {
    FloatIEEE754 f;

    // NaN
    float nan = std::numeric_limits<float>::quiet_NaN();
    f.fromDecimal(nan);
    EXPECT_TRUE(std::isnan(f.toDecimal()));

    // Бесконечность
    float inf = std::numeric_limits<float>::infinity();
    f.fromDecimal(inf);
    EXPECT_TRUE(std::isinf(f.toDecimal()));
    EXPECT_GT(f.toDecimal(), 0);

    // -Бесконечность
    f.fromDecimal(-inf);
    EXPECT_TRUE(std::isinf(f.toDecimal()));
    EXPECT_LT(f.toDecimal(), 0);
}

TEST_F(FloatIEEE754Test, DenormalNumbers) {
    FloatIEEE754 f;

    // Минимальное денормализованное число
    float denorm = 1.4e-45f;
    f.fromDecimal(denorm);
    float result = f.toDecimal();
    EXPECT_GT(result, 0);
    EXPECT_LT(result, 1.18e-38f);

    // Проверка точности
    f.fromDecimal(1.0e-40f);
    result = f.toDecimal();
    EXPECT_GT(result, 0);
}

TEST_F(FloatIEEE754Test, Overflow) {
    FloatIEEE754 a, b;

    // Умножение приводит к переполнению
    a.fromDecimal(1.0e20f);
    b.fromDecimal(1.0e20f);

    FloatIEEE754 result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));
}

TEST_F(FloatIEEE754Test, Underflow) {
    FloatIEEE754 a, b;

    // Умножение очень маленьких чисел
    a.fromDecimal(1.0e-20f);
    b.fromDecimal(1.0e-20f);

    FloatIEEE754 result = FloatIEEE754::multiply(a, b);
    float res = result.toDecimal();

    // Проверяем что результат очень маленький (денормализованное число)
    EXPECT_LT(res, 1.0e-30f);
    EXPECT_GE(res, 0.0f);
}

TEST_F(FloatIEEE754Test, DivideSpecialCases) {
    FloatIEEE754 a, b, result;

    // Деление на ноль
    a.fromDecimal(1.0f);
    b.fromDecimal(0.0f);
    result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));

    // 0 / 0 = NaN
    a.fromDecimal(0.0f);
    b.fromDecimal(0.0f);
    result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // ∞ / ∞ = NaN
    float inf = std::numeric_limits<float>::infinity();
    a.fromDecimal(inf);
    b.fromDecimal(inf);
    result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // ∞ / X = ∞
    a.fromDecimal(inf);
    b.fromDecimal(1.0f);
    result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));

    // X / ∞ = 0
    a.fromDecimal(1.0f);
    b.fromDecimal(inf);
    result = FloatIEEE754::divide(a, b);
    EXPECT_EQ(result.toDecimal(), 0.0f);
}

TEST_F(FloatIEEE754Test, MultiplySpecialCases) {
    FloatIEEE754 a, b, result;

    float inf = std::numeric_limits<float>::infinity();

    // ∞ * 0 = NaN
    a.fromDecimal(inf);
    b.fromDecimal(0.0f);
    result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // 0 * ∞ = NaN
    a.fromDecimal(0.0f);
    b.fromDecimal(inf);
    result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // ∞ * X = ∞
    a.fromDecimal(inf);
    b.fromDecimal(2.0f);
    result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));

    // X * ∞ = ∞
    a.fromDecimal(2.0f);
    b.fromDecimal(inf);
    result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));
}

TEST_F(FloatIEEE754Test, AddSpecialCases) {
    FloatIEEE754 a, b, result;

    float inf = std::numeric_limits<float>::infinity();

    // ∞ + (-∞) = NaN
    a.fromDecimal(inf);
    b.fromDecimal(-inf);
    result = FloatIEEE754::add(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // ∞ + X = ∞
    a.fromDecimal(inf);
    b.fromDecimal(1.0f);
    result = FloatIEEE754::add(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));

    // -∞ + X = -∞
    a.fromDecimal(-inf);
    b.fromDecimal(1.0f);
    result = FloatIEEE754::add(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));
    EXPECT_LT(result.toDecimal(), 0);
}

TEST_F(FloatIEEE754Test, ExtremeValues) {
    FloatIEEE754 f;

    // Максимальное нормализованное число
    float maxFloat = std::numeric_limits<float>::max();
    f.fromDecimal(maxFloat);
    EXPECT_FLOAT_EQ(f.toDecimal(), maxFloat);

    // Минимальное нормализованное положительное
    float minFloat = std::numeric_limits<float>::min();
    f.fromDecimal(minFloat);
    EXPECT_FLOAT_EQ(f.toDecimal(), minFloat);

    // Эпсилон
    float epsilon = std::numeric_limits<float>::epsilon();
    f.fromDecimal(epsilon);
    EXPECT_FLOAT_EQ(f.toDecimal(), epsilon);
}

TEST_F(FloatIEEE754Test, Precision) {
    // Тест на точность для чисел, которые плохо представляются в двоичном виде
    FloatIEEE754 f;

    f.fromDecimal(0.1f);
    float result = f.toDecimal();
    EXPECT_NEAR(result, 0.1f, 1e-6);

    f.fromDecimal(0.2f);
    result = f.toDecimal();
    EXPECT_NEAR(result, 0.2f, 1e-6);

    f.fromDecimal(0.3f);
    result = f.toDecimal();
    EXPECT_NEAR(result, 0.3f, 1e-6);
}

TEST_F(FloatIEEE754Test, AddUnderflow) {
    FloatIEEE754 a, b;

    // Очень маленькие числа
    a.fromDecimal(1.0e-45f);
    b.fromDecimal(1.0e-45f);

    FloatIEEE754 result = FloatIEEE754::add(a, b);
    float res = result.toDecimal();

    // Должно быть очень маленькое число
    EXPECT_LT(res, 1.0e-40f);
    EXPECT_GT(res, 0.0f);
}

TEST_F(FloatIEEE754Test, MultiplyUnderflow) {
    FloatIEEE754 a, b;

    // Экстремально маленькие числа
    a.fromDecimal(1.0e-30f);
    b.fromDecimal(1.0e-30f);

    FloatIEEE754 result = FloatIEEE754::multiply(a, b);
    float res = result.toDecimal();

    // Должен быть 0 или очень маленькое число
    EXPECT_LE(res, 1.0e-50f);
}

TEST_F(FloatIEEE754Test, MultiplyDenormal) {
    FloatIEEE754 a, b;

    // Денормализованное * нормализованное
    a.fromDecimal(1.4e-45f);  // минимальное денормализованное
    b.fromDecimal(2.0f);

    FloatIEEE754 result = FloatIEEE754::multiply(a, b);
    float res = result.toDecimal();

    // Проверяем что результат положительный
    EXPECT_GT(res, 0.0f);
}

TEST_F(FloatIEEE754Test, DivideDenormal) {
    FloatIEEE754 a, b;

    // Денормализованное / нормализованное
    a.fromDecimal(1.4e-45f);
    b.fromDecimal(2.0f);

    FloatIEEE754 result = FloatIEEE754::divide(a, b);
    float res = result.toDecimal();

    // Может быть 0 или очень маленькое число
    EXPECT_GE(res, 0.0f);
}

TEST_F(FloatIEEE754Test, DivideExtremeValues) {
    FloatIEEE754 a, b;

    // Маленькое / большое = очень маленькое
    a.fromDecimal(1.0e-38f);
    b.fromDecimal(1.0e30f);

    FloatIEEE754 result = FloatIEEE754::divide(a, b);
    float res = result.toDecimal();

    // Результат должен быть очень маленьким или 0
    EXPECT_LE(res, 1.0e-60f);
}

TEST_F(FloatIEEE754Test, AddDenormal) {
    FloatIEEE754 a, b;

    // Денормализованное + маленькое
    a.fromDecimal(1.4e-45f);
    b.fromDecimal(1.0e-40f);

    FloatIEEE754 result = FloatIEEE754::add(a, b);
    float res = result.toDecimal();

    EXPECT_GT(res, 0.0f);
}

TEST_F(FloatIEEE754Test, ExponentBoundaries) {
    FloatIEEE754 f;

    // Максимальная экспонента
    f.fromDecimal(3.402823e+38f);  // близко к максимуму
    EXPECT_GT(f.toDecimal(), 0.0f);

    // Минимальная нормализованная экспонента
    f.fromDecimal(1.175494e-38f);  // минимальное нормализованное
    EXPECT_GT(f.toDecimal(), 0.0f);
}

TEST_F(FloatIEEE754Test, MultiplyExtremeValues) {
    FloatIEEE754 a, b;

    // Максимальное * 2 = бесконечность
    a.fromDecimal(3.402823e+38f);
    b.fromDecimal(2.0f);

    FloatIEEE754 result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));

    // Минимальное * 0.5 = денормализованное
    a.fromDecimal(1.175494e-38f);
    b.fromDecimal(0.5f);

    result = FloatIEEE754::multiply(a, b);
    EXPECT_LT(result.toDecimal(), 1.175494e-38f);
}

TEST_F(FloatIEEE754Test, ZeroOperations) {
    FloatIEEE754 a, b, result;

    // 0 + 0 = 0
    a.fromDecimal(0.0f);
    b.fromDecimal(0.0f);
    result = FloatIEEE754::add(a, b);
    EXPECT_EQ(result.toDecimal(), 0.0f);

    // 0 - 0 = 0
    result = FloatIEEE754::subtract(a, b);
    EXPECT_EQ(result.toDecimal(), 0.0f);

    // 0 * X = 0
    b.fromDecimal(5.0f);
    result = FloatIEEE754::multiply(a, b);
    EXPECT_EQ(result.toDecimal(), 0.0f);

    // X * 0 = 0
    result = FloatIEEE754::multiply(b, a);
    EXPECT_EQ(result.toDecimal(), 0.0f);

    // 0 / X = 0
    result = FloatIEEE754::divide(a, b);
    EXPECT_EQ(result.toDecimal(), 0.0f);
}

TEST_F(FloatIEEE754Test, SignOperations) {
    FloatIEEE754 a, b, result;

    // Отрицательное * отрицательное = положительное
    a.fromDecimal(-5.0f);
    b.fromDecimal(-3.0f);
    result = FloatIEEE754::multiply(a, b);
    EXPECT_GT(result.toDecimal(), 0.0f);

    // Отрицательное / отрицательное = положительное
    result = FloatIEEE754::divide(a, b);
    EXPECT_GT(result.toDecimal(), 0.0f);

    // Отрицательное + отрицательное = отрицательное
    result = FloatIEEE754::add(a, b);
    EXPECT_LT(result.toDecimal(), 0.0f);

    // Положительное - отрицательное = положительное
    a.fromDecimal(5.0f);
    b.fromDecimal(-3.0f);
    result = FloatIEEE754::subtract(a, b);
    EXPECT_GT(result.toDecimal(), 0.0f);
}

TEST_F(FloatIEEE754Test, NaNPropagation) {
    FloatIEEE754 a, b, result;

    float nan = std::numeric_limits<float>::quiet_NaN();
    float inf = std::numeric_limits<float>::infinity();

    // NaN + X = NaN
    a.fromDecimal(nan);
    b.fromDecimal(5.0f);
    result = FloatIEEE754::add(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // X + NaN = NaN
    result = FloatIEEE754::add(b, a);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // NaN * X = NaN
    result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // NaN / X = NaN
    result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    // ∞ + NaN = NaN
    a.fromDecimal(inf);
    result = FloatIEEE754::add(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));  // ∞ + X = ∞
}

TEST_F(FloatIEEE754Test, InfinityOperations) {
    FloatIEEE754 a, b, result;

    float inf = std::numeric_limits<float>::infinity();


    a.fromDecimal(inf);
    b.fromDecimal(inf);
    result = FloatIEEE754::add(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));
    EXPECT_GT(result.toDecimal(), 0.0f);

 
    result = FloatIEEE754::subtract(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));

    result = FloatIEEE754::multiply(a, b);
    EXPECT_TRUE(std::isinf(result.toDecimal()));

    // ∞ / ∞ = NaN
    result = FloatIEEE754::divide(a, b);
    EXPECT_TRUE(std::isnan(result.toDecimal()));
}
