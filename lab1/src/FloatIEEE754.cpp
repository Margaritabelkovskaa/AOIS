#include "FloatIEEE754.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace FloatConstants {
    const int BITS_COUNT = 32;
    const int SIGN_BIT = 31;
    const int EXPONENT_BITS = 8;
    const int MANTISSA_BITS = 23;
    const int EXPONENT_BIAS = 127;
    const int MAX_EXPONENT = 255;
    const int MANTISSA_IMPLICIT_BIT = 23;
    const int MANTISSA_EXTENDED_BITS = 24;
    const int MULTIPLICATION_EXTRA_BITS = 47;
    const int MAX_NORMALIZATION_SHIFT = 32;
}

FloatIEEE754::FloatIEEE754() {
    for (int i = 0; i < FloatConstants::BITS_COUNT; i++) bits[i] = 0;
}

void FloatIEEE754::fromDecimal(float num) {
    union { float f; unsigned int i; } u;
    u.f = num;
    for (int i = 0; i < FloatConstants::BITS_COUNT; i++)
        bits[i] = (u.i >> i) & 1;
}

float FloatIEEE754::toDecimal() const {
    union { float f; unsigned int i; } u;
    u.i = 0;
    for (int i = 0; i < FloatConstants::BITS_COUNT; i++)
        if (bits[i]) u.i |= (1 << i);
    return u.f;
}

void FloatIEEE754::printBinary() const {
    for (int i = FloatConstants::BITS_COUNT - 1; i >= 0; i--) {
        cout << bits[i];
        if (i == FloatConstants::SIGN_BIT || i == FloatConstants::MANTISSA_BITS)
            cout << " ";
    }
}

int FloatIEEE754::getExponent() const {
    int exp = 0;
    for (int i = 0; i < FloatConstants::EXPONENT_BITS; i++)
        if (bits[FloatConstants::MANTISSA_BITS + i]) exp |= (1 << i);
    return exp;
}

unsigned int FloatIEEE754::getMantissa() const {
    unsigned int mant = 0;
    for (int i = 0; i < FloatConstants::MANTISSA_BITS; i++)
        if (bits[i]) mant |= (1 << i);
    return mant;
}

void FloatIEEE754::setExponent(int e) {
    for (int i = 0; i < FloatConstants::EXPONENT_BITS; i++)
        bits[FloatConstants::MANTISSA_BITS + i] = (e >> i) & 1;
}

void FloatIEEE754::setMantissa(unsigned int m) {
    for (int i = 0; i < FloatConstants::MANTISSA_BITS; i++)
        bits[i] = (m >> i) & 1;
}

bool FloatIEEE754::isNaN() const {
    return (getExponent() == FloatConstants::MAX_EXPONENT && getMantissa() != 0);
}

bool FloatIEEE754::isInfinity() const {
    return (getExponent() == FloatConstants::MAX_EXPONENT && getMantissa() == 0);
}

bool FloatIEEE754::isZero() const {
    return (getExponent() == 0 && getMantissa() == 0);
}

bool FloatIEEE754::isDenormal() const {
    return (getExponent() == 0 && getMantissa() != 0);
}


FloatIEEE754 FloatIEEE754::add(const FloatIEEE754& a, const FloatIEEE754& b) {
    // Извлекаем компоненты
    int signA = a.getSign();
    int signB = b.getSign();
    int expA = a.getExponent();
    int expB = b.getExponent();
    unsigned int mantA = a.getMantissa();
    unsigned int mantB = b.getMantissa();
    
    FloatIEEE754 result;
    
    // Проверка специальных случаев
    if (a.isNaN()) return a;
    if (b.isNaN()) return b;
    
    if (a.isInfinity()) {
        if (b.isInfinity() && a.getSign() != b.getSign()) {
            result.setExponent(FloatConstants::MAX_EXPONENT);
            result.setMantissa(1);
            return result;
        }
        return a;
    }
    if (b.isInfinity()) return b;
    
    // Добавляем неявную единицу
    unsigned long long mantLongA = mantA;
    unsigned long long mantLongB = mantB;
    if (expA != 0) mantLongA |= (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT);
    if (expB != 0) mantLongB |= (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT);
    
    // Выравнивание экспонент
    int expRes;
    if (expA > expB) {
        mantLongB >>= (expA - expB);
        expRes = expA;
    } else if (expB > expA) {
        mantLongA >>= (expB - expA);
        expRes = expB;
    } else {
        expRes = expA;
    }
    
    // Сложение/вычитание мантисс
    unsigned long long mantRes;
    int signRes;
    
    if (signA == signB) {
        mantRes = mantLongA + mantLongB;
        signRes = signA;
        if (mantRes & (1ULL << FloatConstants::MANTISSA_EXTENDED_BITS)) {
            mantRes >>= 1;
            expRes++;
        }
    } else {
        if (mantLongA >= mantLongB) {
            mantRes = mantLongA - mantLongB;
            signRes = signA;
        } else {
            mantRes = mantLongB - mantLongA;
            signRes = signB;
        }
        if (mantRes != 0) {
            while ((mantRes & (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT)) == 0) {
                mantRes <<= 1;
                expRes--;
            }
        }
    }
    
    // Проверка переполнения
    if (expRes >= FloatConstants::MAX_EXPONENT) {
        result.setSign(signRes);
        result.setExponent(FloatConstants::MAX_EXPONENT);
        result.setMantissa(0);
        return result;
    }
    
    // Проверка underflow
    if (expRes <= 0) {
        if (expRes <= -FloatConstants::MANTISSA_EXTENDED_BITS || mantRes == 0) {
            result.setSign(signRes);
            result.setExponent(0);
            result.setMantissa(0);
            return result;
        }
        mantRes >>= (1 - expRes);
        expRes = 0;
    }
    
    // Убираем неявную единицу
    unsigned int finalMant;
    if (expRes != 0)
        finalMant = (unsigned int)(mantRes & ~(1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT));
    else
        finalMant = (unsigned int)mantRes;
    
    result.setSign(signRes);
    result.setExponent(expRes);
    result.setMantissa(finalMant);
    return result;
}

FloatIEEE754 FloatIEEE754::subtract(const FloatIEEE754& a, const FloatIEEE754& b) {
    FloatIEEE754 negB = b;
    negB.setSign(!b.getSign());
    return add(a, negB);
}


int getRealExponent(int exp) {
    return (exp == 0) ? -126 : exp - FloatConstants::EXPONENT_BIAS;
}

FloatIEEE754 FloatIEEE754::multiply(const FloatIEEE754& a, const FloatIEEE754& b) {
    int signA = a.getSign(), signB = b.getSign();
    int expA = a.getExponent(), expB = b.getExponent();
    unsigned int mantA = a.getMantissa(), mantB = b.getMantissa();
    int signRes = signA ^ signB;
    FloatIEEE754 result;
    
    if (a.isNaN()) return a;
    if (b.isNaN()) return b;
    
    if (a.isInfinity()) {
        if (b.isZero()) {
            result.setExponent(FloatConstants::MAX_EXPONENT);
            result.setMantissa(1);
            return result;
        }
        result.setSign(signRes);
        result.setExponent(FloatConstants::MAX_EXPONENT);
        result.setMantissa(0);
        return result;
    }
    
    if (b.isInfinity()) {
        if (a.isZero()) {
            result.setExponent(FloatConstants::MAX_EXPONENT);
            result.setMantissa(1);
            return result;
        }
        result.setSign(signRes);
        result.setExponent(FloatConstants::MAX_EXPONENT);
        result.setMantissa(0);
        return result;
    }
    
    if (a.isZero() || b.isZero()) {
        result.setSign(signRes);
        result.setExponent(0);
        result.setMantissa(0);
        return result;
    }
    
    int realExpA = getRealExponent(expA);
    int realExpB = getRealExponent(expB);
    
    unsigned long long mantLongA = mantA;
    unsigned long long mantLongB = mantB;
    if (expA != 0) mantLongA |= (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT);
    if (expB != 0) mantLongB |= (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT);
    
    unsigned long long mantRes = mantLongA * mantLongB;
    int expRes = realExpA + realExpB + FloatConstants::EXPONENT_BIAS;
    
    if (mantRes & (1ULL << FloatConstants::MULTIPLICATION_EXTRA_BITS)) {
        mantRes >>= FloatConstants::MANTISSA_EXTENDED_BITS;
        expRes++;
    } else {
        mantRes >>= FloatConstants::MANTISSA_IMPLICIT_BIT;
    }
 
    if (expRes >= FloatConstants::MAX_EXPONENT) {
        result.setSign(signRes);
        result.setExponent(FloatConstants::MAX_EXPONENT);
        result.setMantissa(0);
        return result;
    }
  
    if (expRes <= 0) {
        if (expRes <= -FloatConstants::MANTISSA_EXTENDED_BITS) {
            result.setSign(signRes);
            result.setExponent(0);
            result.setMantissa(0);
            return result;
        }
        mantRes >>= (1 - expRes);
        expRes = 0;
    }
    
    // Убираем неявную единицу
    unsigned int finalMant;
    if (expRes != 0)
        finalMant = (unsigned int)(mantRes & ~(1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT));
    else
        finalMant = (unsigned int)mantRes;
    
    result.setSign(signRes);
    result.setExponent(expRes);
    result.setMantissa(finalMant);
    return result;
}


bool checkDivideSpecialCases(const FloatIEEE754& a, const FloatIEEE754& b,
                              int signRes, FloatIEEE754& result) {
    if (a.isNaN()) { result = a; return true; }
    if (b.isNaN()) { result = b; return true; }
    
    if (b.isZero()) {
        if (a.isZero()) {
            result.setExponent(FloatConstants::MAX_EXPONENT);
            result.setMantissa(1);
        } else {
            result.setSign(signRes);
            result.setExponent(FloatConstants::MAX_EXPONENT);
            result.setMantissa(0);
        }
        return true;
    }
    
    if (a.isInfinity()) {
        if (b.isInfinity()) {
            result.setExponent(FloatConstants::MAX_EXPONENT);
            result.setMantissa(1);
        } else {
            result.setSign(signRes);
            result.setExponent(FloatConstants::MAX_EXPONENT);
            result.setMantissa(0);
        }
        return true;
    }
    
    if (b.isInfinity() || a.isZero()) {
        result.setSign(signRes);
        result.setExponent(0);
        result.setMantissa(0);
        return true;
    }
    return false;
}

unsigned long long addImplicitBit(unsigned int mant, int exp) {
    if (exp == 0) return mant;
    return mant | (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT);
}

int normalizeDividend(unsigned long long& dividend, unsigned long long divisor) {
    int shift = 0;
    while (dividend < divisor && shift < FloatConstants::MAX_NORMALIZATION_SHIFT) {
        dividend <<= 1;
        shift++;
    }
    return shift;
}

unsigned long long performDivision(unsigned long long dividend, unsigned long long divisor) {
    unsigned long long quotient = 0;
    unsigned long long remainder = dividend;
    
    for (int i = 0; i < FloatConstants::MANTISSA_EXTENDED_BITS; i++) {
        quotient <<= 1;
        if (remainder >= divisor) {
            quotient |= 1;
            remainder -= divisor;
        }
        remainder <<= 1;
    }
    return quotient;
}

void normalizeQuotient(unsigned long long& quotient, int& expRes) {
    if (quotient & (1ULL << FloatConstants::MANTISSA_EXTENDED_BITS)) {
        quotient >>= 1;
        expRes++;
    } else if ((quotient & (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT)) == 0) {
        int normShift = 0;
        while ((quotient & (1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT)) == 0 && quotient != 0) {
            quotient <<= 1;
            normShift++;
        }
        expRes -= normShift;
    }
}

unsigned int removeImplicitBit(unsigned long long mant, int exp) {
    if (exp == 0) return (unsigned int)mant;
    return (unsigned int)(mant & ~(1ULL << FloatConstants::MANTISSA_IMPLICIT_BIT));
}

bool checkDivideOverflow(int expRes, int signRes, FloatIEEE754& result) {
    if (expRes >= FloatConstants::MAX_EXPONENT) {
        result.setSign(signRes);
        result.setExponent(FloatConstants::MAX_EXPONENT);
        result.setMantissa(0);
        return true;
    }
    return false;
}

bool checkDivideUnderflow(int& expRes, unsigned long long& quotient,
                          int signRes, FloatIEEE754& result) {
    if (expRes <= 0) {
        if (expRes <= -FloatConstants::MANTISSA_EXTENDED_BITS || quotient == 0) {
            result.setSign(signRes);
            result.setExponent(0);
            result.setMantissa(0);
            return true;
        }
        quotient >>= (1 - expRes);
        expRes = 0;
    }
    return false;
}

FloatIEEE754 FloatIEEE754::divide(const FloatIEEE754& a, const FloatIEEE754& b) {
    int signA = a.getSign();
    int signB = b.getSign();
    int expA = a.getExponent();
    int expB = b.getExponent();
    unsigned int mantA = a.getMantissa();
    unsigned int mantB = b.getMantissa();
    int signRes = signA ^ signB;
    FloatIEEE754 result;
    if (checkDivideSpecialCases(a, b, signRes, result)) return result;
    int realExpA = getRealExponent(expA);
    int realExpB = getRealExponent(expB);
    
    unsigned long long mantLongA = addImplicitBit(mantA, expA);
    unsigned long long mantLongB = addImplicitBit(mantB, expB);
    
    int shift = normalizeDividend(mantLongA, mantLongB);
    unsigned long long quotient = performDivision(mantLongA, mantLongB);
    if (shift > 0) quotient >>= shift;

    int expRes = realExpA - realExpB + FloatConstants::EXPONENT_BIAS;
    normalizeQuotient(quotient, expRes);

    if (checkDivideOverflow(expRes, signRes, result)) return result;
    if (checkDivideUnderflow(expRes, quotient, signRes, result)) return result;
    
    quotient &= ((1ULL << FloatConstants::MANTISSA_EXTENDED_BITS) - 1);
    unsigned int finalMant = removeImplicitBit(quotient, expRes);
    
    result.setSign(signRes);
    result.setExponent(expRes);
    result.setMantissa(finalMant);
    return result;
}
