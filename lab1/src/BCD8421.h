#ifndef BCD8421_H
#define BCD8421_H

#include <iostream>
using namespace std;

class BCD8421 {
private:
    static const int DIGITS_COUNT = 8;
    static const int BITS_PER_DIGIT = 4;
    static const int SIGN_TETRAD = 0;
    static const int FIRST_DIGIT = 1;
    static const int LAST_DIGIT = 7;
    static const unsigned char NEGATIVE_SIGN = 0x0F;
    static const unsigned char POSITIVE_SIGN = 0;
    static const int BCD_BASE = 10;
    static const int BCD_MAX_DIGIT = 9;
    static const unsigned int MIN_INT_ABS = 2147483648U;

    unsigned char digits[DIGITS_COUNT];

    static unsigned char toBCD(unsigned char value);
    static unsigned char fromBCD(unsigned char bcd);
    
    bool isNegative() const;
    void setSign(bool negative);
    void clearDigits();
    void invertDigits();
    bool addOne();
    bool subtractOne();
    void writeAbsoluteValue(unsigned int absNum);
    
    void toAdditionalCode();
    void fromAdditionalToDirect();

public:
    BCD8421();
    
    void fromDecimal(int num);
    int toDecimal() const;
    void printBCD() const;
    
    static int addTetrad(int digitA, int digitB, int carry, unsigned char& resultDigit);
    static void printTetradInfo(int i, int digitA, int digitB, int carry, int sum, int newCarry);
    static void printAdditionHeader(const BCD8421& a, const BCD8421& b);
    static BCD8421 hardwareAdd(const BCD8421& a, const BCD8421& b);
};

#endif
