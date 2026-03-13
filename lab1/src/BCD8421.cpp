#include "BCD8421.h"
#include <iostream>
#include <climits>
using namespace std;

BCD8421::BCD8421() {
    clearDigits();
}

void BCD8421::clearDigits() {
    for (int i = 0; i < DIGITS_COUNT; i++) digits[i] = 0;
}

bool BCD8421::isNegative() const {
    return (digits[SIGN_TETRAD] & 0x0F) == NEGATIVE_SIGN;
}

void BCD8421::setSign(bool negative) {
    digits[SIGN_TETRAD] = negative ? NEGATIVE_SIGN : POSITIVE_SIGN;
}

unsigned char BCD8421::toBCD(unsigned char value) {
    return value & 0x0F;
}

unsigned char BCD8421::fromBCD(unsigned char bcd) {
    return bcd & 0x0F;
}

void BCD8421::invertDigits() {
    for (int i = FIRST_DIGIT; i <= LAST_DIGIT; i++) {
        digits[i] = toBCD(BCD_MAX_DIGIT - fromBCD(digits[i]));
    }
}

bool BCD8421::addOne() {
    int carry = 1;
    for (int i = LAST_DIGIT; i >= FIRST_DIGIT; i--) {
        int val = fromBCD(digits[i]) + carry;
        if (val >= BCD_BASE) {
            digits[i] = toBCD(val - BCD_BASE);
            carry = 1;
        } else {
            digits[i] = toBCD(val);
            carry = 0;
            break;
        }
    }
    return (carry == 1);
}

bool BCD8421::subtractOne() {
    int borrow = 1;
    for (int i = LAST_DIGIT; i >= FIRST_DIGIT; i--) {
        int val = fromBCD(digits[i]) - borrow;
        if (val < 0) {
            digits[i] = toBCD(val + BCD_BASE);
            borrow = 1;
        } else {
            digits[i] = toBCD(val);
            borrow = 0;
            break;
        }
    }
    return (borrow == 1);
}

void BCD8421::toAdditionalCode() {
    if (isNegative()) {
        invertDigits();
        addOne();
    }
}

void BCD8421::fromAdditionalToDirect() {
    if (isNegative()) {
        subtractOne();
        invertDigits();
    }
}

void BCD8421::writeAbsoluteValue(unsigned int absNum) {
    unsigned int temp = absNum;
    for (int i = LAST_DIGIT; i >= FIRST_DIGIT; i--) {
        if (temp > 0) {
            digits[i] = toBCD(temp % BCD_BASE);
            temp /= BCD_BASE;
        } else {
            digits[i] = 0;
        }
    }
}

void BCD8421::fromDecimal(int num) {
    clearDigits();
    
    if (num == 0) {
        digits[SIGN_TETRAD] = POSITIVE_SIGN;
        return;
    }
    
    bool negative = (num < 0);
    unsigned int absNum = (num == INT_MIN) ? MIN_INT_ABS : ((num < 0) ? -num : num);
    
    setSign(negative);
    writeAbsoluteValue(absNum);
    
    if (negative) toAdditionalCode();
}

int BCD8421::toDecimal() const {
    BCD8421 temp = *this;
    bool negative = temp.isNegative();
    
    if (negative) temp.fromAdditionalToDirect();
    
    long long result = 0;
    for (int i = FIRST_DIGIT; i <= LAST_DIGIT; i++) {
        result = result * BCD_BASE + fromBCD(temp.digits[i]);
    }
    
    return negative ? -result : result;
}

void BCD8421::printBCD() const {
    for (int i = 0; i < DIGITS_COUNT; i++) {
        for (int j = BITS_PER_DIGIT - 1; j >= 0; j--) {
            cout << ((digits[i] >> j) & 1);
        }
        if (i < DIGITS_COUNT - 1) cout << " ";
    }
    cout << " (" << toDecimal() << ")";
}

int BCD8421::addTetrad(int digitA, int digitB, int carry, unsigned char& resultDigit) {
    int sum = digitA + digitB + carry;
    
    if (sum >= BCD_BASE) {
        resultDigit = toBCD(sum - BCD_BASE);
        return 1;
    } else {
        resultDigit = toBCD(sum);
        return 0;
    }
}

void BCD8421::printTetradInfo(int i, int digitA, int digitB, int carry, int sum, int newCarry) {
    cout << "Nibble " << i << ": " << digitA << " + " << digitB << " + carry " << carry << " = ";
    if (sum >= BCD_BASE) {
        cout << sum - BCD_BASE << ", carry = " << newCarry;
    } else {
        cout << sum << ", carry = " << newCarry;
    }
    cout << endl;
}

void BCD8421::printAdditionHeader(const BCD8421& a, const BCD8421& b) {
    cout << "\n=== HARDWARE BCD ADDITION ===\n";
    cout << "A: "; a.printBCD(); cout << endl;
    cout << "B: "; b.printBCD(); cout << endl;
    cout << "-------------------------------\n";
}

BCD8421 BCD8421::hardwareAdd(const BCD8421& a, const BCD8421& b) {
    BCD8421 result;
    result.digits[SIGN_TETRAD] = a.digits[SIGN_TETRAD];
    
    int carry = 0;
    printAdditionHeader(a, b);
    
    for (int i = LAST_DIGIT; i >= FIRST_DIGIT; i--) {
        int digitA = fromBCD(a.digits[i]);
        int digitB = fromBCD(b.digits[i]);
        int newCarry = addTetrad(digitA, digitB, carry, result.digits[i]);
        
        printTetradInfo(i, digitA, digitB, carry, digitA + digitB + carry, newCarry);
        carry = newCarry;
    }
    
    if (carry == 1) {
        cout << "WARNING: Overflow! Result does not fit in " << LAST_DIGIT << " digits\n";
    }
    
    cout << "Result: ";
    result.printBCD();
    cout << endl;
    
    return result;
}
