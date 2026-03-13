#include "BinaryCode.h"
#include <iostream>
#include <cmath>
using namespace std;

BinaryCode::BinaryCode() {
    for (int i = 0; i < BITS_COUNT; i++) bits[i] = 0;
}

int BinaryCode::getBit(int index) const {
    return (index >= 0 && index < BITS_COUNT) ? bits[index] : 0;
}

void BinaryCode::setBit(int index, int value) {
    if (index >= 0 && index < BITS_COUNT) bits[index] = (value ? 1 : 0);
}

void BinaryCode::printBinary() const {
    for (int i = BITS_COUNT - 1; i >= 0; i--) {
        cout << bits[i];
        if (i % 4 == 0 && i > 0) cout << " ";
    }
}

void BinaryCode::copyFrom(const BinaryCode& other) {
    for (int i = 0; i < BITS_COUNT; i++) bits[i] = other.getBit(i);
}

void BinaryCode::fromDecimalToDirect(int num) {
    for (int i = 0; i < BITS_COUNT; i++) bits[i] = 0;
    
    if (num < 0) {
        bits[SIGN_BIT] = 1;
        num = -num;
    }
    
    int index = 0;
    while (num > 0 && index < MAGNITUDE_BITS) {
        bits[index] = num % 2;
        num /= 2;
        index++;
    }
}

int BinaryCode::fromDirectToDecimal() const {
    int result = 0;
    for (int i = MAGNITUDE_BITS - 1; i >= 0; i--) {
        result = result * 2 + bits[i];
    }
    return (bits[SIGN_BIT] == 1) ? -result : result;
}

void BinaryCode::invertBits() {
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        bits[i] = (bits[i] ? 0 : 1);
    }
}

void BinaryCode::addOne() {
    int carry = 1;
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        int sum = bits[i] + carry;
        bits[i] = sum % 2;
        carry = sum / 2;
        if (carry == 0) break;
    }
}

void BinaryCode::subtractOne() {
    int borrow = 1;
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        int diff = bits[i] - borrow;
        if (diff < 0) {
            bits[i] = 1;
            borrow = 1;
        } else {
            bits[i] = diff;
            borrow = 0;
            break;
        }
    }
}

void BinaryCode::toAdditionalCode() {
    if (bits[SIGN_BIT] == 1) {
        invertBits();
        addOne();
    }
}

void BinaryCode::toReverseCode() {
    if (bits[SIGN_BIT] == 1) invertBits();
}

void BinaryCode::fromAdditionalToDirect() {
    if (bits[SIGN_BIT] == 1) {
        subtractOne();
        invertBits();
    }
}

BinaryCode BinaryCode::getTwosComplement(const BinaryCode& b) {
    BinaryCode minusB;
    minusB.copyFrom(b);
    
    for (int i = 0; i < BITS_COUNT; i++) {
        minusB.setBit(i, b.getBit(i) ? 0 : 1);
    }
    
    int carry = 1;
    for (int i = 0; i < BITS_COUNT; i++) {
        int sum = minusB.getBit(i) + carry;
        minusB.setBit(i, sum % 2);
        carry = sum / 2;
    }
    return minusB;
}

BinaryCode BinaryCode::addAdditional(const BinaryCode& a, const BinaryCode& b) {
    BinaryCode result;
    int carry = 0;
    
    for (int i = 0; i < BITS_COUNT; i++) {
        int sum = a.getBit(i) + b.getBit(i) + carry;
        result.setBit(i, sum % 2);
        carry = sum / 2;
    }
    return result;
}

BinaryCode BinaryCode::subtractAdditional(const BinaryCode& a, const BinaryCode& b) {
    return addAdditional(a, getTwosComplement(b));
}

BinaryCode BinaryCode::binaryAdd(const BinaryCode& a, const BinaryCode& b) {
    BinaryCode result;
    int carry = 0;
    
    for (int i = 0; i < BITS_COUNT; i++) {
        int sum = a.getBit(i) + b.getBit(i) + carry;
        result.setBit(i, sum % 2);
        carry = sum / 2;
    }
    return result;
}

BinaryCode BinaryCode::binarySubtract(const BinaryCode& a, const BinaryCode& b) {
    BinaryCode result;
    int borrow = 0;
    
    for (int i = 0; i < BITS_COUNT; i++) {
        int diff = a.getBit(i) - b.getBit(i) - borrow;
        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.setBit(i, diff);
    }
    return result;
}

bool BinaryCode::isGreaterOrEqual(const BinaryCode& a, const BinaryCode& b) {
    if (a.getBit(SIGN_BIT) != b.getBit(SIGN_BIT)) {
        return a.getBit(SIGN_BIT) == 0;
    }
    
    for (int i = MAGNITUDE_BITS - 1; i >= 0; i--) {
        if (a.getBit(i) > b.getBit(i)) return true;
        if (a.getBit(i) < b.getBit(i)) return false;
    }
    return true;
}

void BinaryCode::shiftLeft() {
    for (int i = MAGNITUDE_BITS - 1; i >= 0; i--) {
        bits[i + 1] = bits[i];
    }
    bits[0] = 0;
}

void BinaryCode::shiftRight() {
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        bits[i] = bits[i + 1];
    }
    bits[MAGNITUDE_BITS] = 0;
}

BinaryCode BinaryCode::multiplyDirect(const BinaryCode& a, const BinaryCode& b) {
    int signResult = a.getBit(SIGN_BIT) ^ b.getBit(SIGN_BIT);
    
    BinaryCode aMag, bMag;
    aMag.copyFrom(a);
    bMag.copyFrom(b);
    aMag.setBit(SIGN_BIT, 0);
    bMag.setBit(SIGN_BIT, 0);
    
    BinaryCode result;
    
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        if (bMag.getBit(i) == 1) {
            BinaryCode shifted;
            shifted.copyFrom(aMag);
            
            for (int s = 0; s < i; s++) {
                shifted.shiftLeft();
            }
            
            result = binaryAdd(result, shifted);
        }
    }
    
    result.setBit(SIGN_BIT, signResult);
    return result;
}

bool BinaryCode::isZero() const {
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        if (bits[i] != 0) return false;
    }
    return true;
}

bool BinaryCode::isDivisorZero(const BinaryCode& divisor) {
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        if (divisor.getBit(i) != 0) return false;
    }
    return true;
}

int BinaryCode::getIntegerPart(const BinaryCode& quotient) {
    int result = 0;
    for (int i = MAGNITUDE_BITS - 1; i >= 0; i--) {
        result = result * 2 + quotient.getBit(i);
    }
    return result;
}

double BinaryCode::getFractionalPart(const BinaryCode& remainder, const BinaryCode& divisor) {
    if (remainder.isZero()) return 0.0;
    
    double fraction = 0.0;
    double weight = 0.5;
    BinaryCode temp;
    temp.copyFrom(remainder);
    
    for (int i = 0; i < FRACTION_ITERATIONS; i++) {
        temp.shiftLeft();
        
        if (isGreaterOrEqual(temp, divisor)) {
            temp = binarySubtract(temp, divisor);
            fraction += weight;
        }
        
        weight /= 2.0;
        if (temp.isZero()) break;
    }
    
    return fraction;
}

void BinaryCode::printDivisionInfo(const BinaryCode& dividend, const BinaryCode& divisor) {
    cout << "\n=== RESTORING DIVISION ===\n";
    cout << "Dividend: "; dividend.printBinary();
    cout << " (" << dividend.fromDirectToDecimal() << ")\n";
    cout << "Divisor: "; divisor.printBinary();
    cout << " (" << divisor.fromDirectToDecimal() << ")\n\n";
}

void BinaryCode::printResult(int intPart, double fraction, int signResult, double decimalResult) {
    cout << "\nRESULT:\n";
    cout << "  Integer part (absolute): " << intPart << "\n";
    cout << "  Fractional part: " << fraction << "\n";
    cout << "  Sign: " << (signResult == 1 ? "-" : "+") << "\n";
    cout << "  Result: " << decimalResult << "\n";
}

BinaryCode BinaryCode::divideRestoring(const BinaryCode& a, const BinaryCode& b,
                                        double& decimalResult,
                                        BinaryCode& remainder_out) {
    BinaryCode quotient;
    int signResult = a.getBit(SIGN_BIT) ^ b.getBit(SIGN_BIT);
    
    BinaryCode dividend, divisor;
    dividend.copyFrom(a);
    divisor.copyFrom(b);
    dividend.setBit(SIGN_BIT, 0);
    divisor.setBit(SIGN_BIT, 0);
    
    if (isDivisorZero(divisor)) {
        cout << "Error: division by zero!" << endl;
        decimalResult = 0;
        return quotient;
    }
    
    printDivisionInfo(dividend, divisor);
    BinaryCode remainder;
    
    for (int i = 0; i < MAGNITUDE_BITS; i++) {
        remainder.shiftLeft();
        int nextBit = dividend.getBit(MAGNITUDE_BITS - 1 - i);
        remainder.setBit(0, nextBit);
        
        BinaryCode prevRemainder;
        prevRemainder.copyFrom(remainder);
        BinaryCode trial = binarySubtract(remainder, divisor);
        
        quotient.shiftLeft();
        
        if (trial.getBit(SIGN_BIT) == 0) {
            remainder.copyFrom(trial);
            quotient.setBit(0, 1);
        } else {
            remainder.copyFrom(prevRemainder);
        }
    }
    
    remainder_out.copyFrom(remainder);
    
    int intPart = getIntegerPart(quotient);
    double fraction = getFractionalPart(remainder, divisor);
    decimalResult = (signResult == 1) ? -(intPart + fraction) : (intPart + fraction);
    
    printResult(intPart, fraction, signResult, decimalResult);
    
    BinaryCode resultQuotient;
    resultQuotient.copyFrom(quotient);
    resultQuotient.setBit(SIGN_BIT, signResult);
    return resultQuotient;
}
