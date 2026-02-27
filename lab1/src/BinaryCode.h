#ifndef BINARYCODE_H
#define BINARYCODE_H
#include <iomanip>
#include <iostream>
using namespace std;

class BinaryCode {
private:
    int bits[32];

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