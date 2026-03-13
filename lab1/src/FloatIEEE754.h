#ifndef FLOATIEEE754_H
#define FLOATIEEE754_H

#include <iostream>
using namespace std;

class FloatIEEE754 {
private:
    static const int BITS_COUNT = 32;
    static const int SIGN_BIT = 31;
    static const int EXPONENT_BITS = 8;
    static const int MANTISSA_BITS = 23;
    static const int EXPONENT_BIAS = 127;
    static const int MAX_EXPONENT = 255;
    static const int MANTISSA_IMPLICIT_BIT = 23;
    static const int MANTISSA_EXTENDED_BITS = 24;
    static const int MULTIPLICATION_EXTRA_BITS = 47;

    int bits[BITS_COUNT];

public:
    FloatIEEE754();

    void fromDecimal(float num);
    float toDecimal() const;
    void printBinary() const;

    static FloatIEEE754 add(const FloatIEEE754& a, const FloatIEEE754& b);
    static FloatIEEE754 subtract(const FloatIEEE754& a, const FloatIEEE754& b);
    static FloatIEEE754 multiply(const FloatIEEE754& a, const FloatIEEE754& b);
    static FloatIEEE754 divide(const FloatIEEE754& a, const FloatIEEE754& b);

    int getSign() const { return bits[SIGN_BIT]; }
    int getExponent() const;
    unsigned int getMantissa() const;
    void setSign(int s) { bits[SIGN_BIT] = s ? 1 : 0; }
    void setExponent(int e);
    void setMantissa(unsigned int m);

    bool isNaN() const;
    bool isInfinity() const;
    bool isZero() const;
    bool isDenormal() const;
};

#endif
