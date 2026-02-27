#ifndef FLOATIEEE754_H
#define FLOATIEEE754_H

#include <iostream>
using namespace std;

class FloatIEEE754 {
private:
    int bits[32];  // 0 - младший бит, 31 - знак

public:
    FloatIEEE754();

    // Преобразование
    void fromDecimal(float num);
    float toDecimal() const;
    void printBinary() const;

    // Арифметические операции
    static FloatIEEE754 add(const FloatIEEE754& a, const FloatIEEE754& b);
    static FloatIEEE754 subtract(const FloatIEEE754& a, const FloatIEEE754& b);
    static FloatIEEE754 multiply(const FloatIEEE754& a, const FloatIEEE754& b);
    static FloatIEEE754 divide(const FloatIEEE754& a, const FloatIEEE754& b);

    // Вспомогательные методы для доступа к битам
    int getSign() const { return bits[31]; }
    int getExponent() const;
    unsigned int getMantissa() const;
    void setSign(int s) { bits[31] = s ? 1 : 0; }
    void setExponent(int e);
    void setMantissa(unsigned int m);
};

#endif