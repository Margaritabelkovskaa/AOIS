#ifndef BINARYCODE_H
#define BINARYCODE_H

#include <iostream>
using namespace std;

class BinaryCode {
private:
    static const int BITS_COUNT = 32;
    static const int SIGN_BIT = 31;
    static const int MAGNITUDE_BITS = 31;
    static const int FRACTION_ITERATIONS = 20;

    int bits[BITS_COUNT];

    void invertBits();
    void addOne();
    void subtractOne();
    static BinaryCode getTwosComplement(const BinaryCode& b);
    bool isZero() const;
    static bool isDivisorZero(const BinaryCode& divisor);
    static int getIntegerPart(const BinaryCode& quotient);
    static double getFractionalPart(const BinaryCode& remainder, const BinaryCode& divisor);
    
    static void printDivisionInfo(const BinaryCode& dividend, const BinaryCode& divisor);
    static void printResult(int intPart, double fraction, int signResult, double decimalResult);

public:
    BinaryCode();

    int getBit(int index) const;
    void setBit(int index, int value);
    void printBinary() const;
    void copyFrom(const BinaryCode& other);

    void fromDecimalToDirect(int num);
    int fromDirectToDecimal() const;

    static BinaryCode binaryAdd(const BinaryCode& a, const BinaryCode& b);
    static BinaryCode binarySubtract(const BinaryCode& a, const BinaryCode& b);
    static bool isGreaterOrEqual(const BinaryCode& a, const BinaryCode& b);

    void shiftLeft();
    void shiftRight();

    static BinaryCode multiplyDirect(const BinaryCode& a, const BinaryCode& b);
    static BinaryCode divideRestoring(const BinaryCode& a, const BinaryCode& b,
                                      double& decimalResult,
                                      BinaryCode& remainder_out);

    void toAdditionalCode();
    void toReverseCode();
    void fromAdditionalToDirect();

    static BinaryCode addAdditional(const BinaryCode& a, const BinaryCode& b);
    static BinaryCode subtractAdditional(const BinaryCode& a, const BinaryCode& b);
};

#endif
   
