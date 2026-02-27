#include "BinaryCode.h"
#include <iostream>
#include <cmath>
using namespace std;

BinaryCode::BinaryCode() {
    for (int i = 0; i < 32; i++) bits[i] = 0;
}

int BinaryCode::getBit(int index) const {
    if (index >= 0 && index < 32) return bits[index];
    return 0;
}

void BinaryCode::setBit(int index, int value) {
    if (index >= 0 && index < 32) bits[index] = (value ? 1 : 0);
}

void BinaryCode::printBinary() const {
    for (int i = 31; i >= 0; i--) {
        cout << bits[i];
        if (i % 4 == 0 && i > 0) cout << " ";
    }
}

void BinaryCode::copyFrom(const BinaryCode& other) {
    for (int i = 0; i < 32; i++) bits[i] = other.getBit(i);
}

void BinaryCode::fromDecimalToDirect(int num) {
    for (int i = 0; i < 32; i++) bits[i] = 0;

    if (num < 0) {
        bits[31] = 1;
        num = -num;
    }

    int index = 0;
    while (num > 0 && index < 31) {
        bits[index] = num % 2;
        num /= 2;
        index++;
    }
}

int BinaryCode::fromDirectToDecimal() const {
    int result = 0;
    for (int i = 30; i >= 0; i--) {
        result = result * 2 + bits[i];
    }
    return (bits[31] == 1) ? -result : result;
}

// ============ МЕТОДЫ ДЛЯ КОДОВ ============

void BinaryCode::toAdditionalCode() {
    if (bits[31] == 1) {
        for (int i = 0; i < 31; i++) {
            bits[i] = (bits[i] ? 0 : 1);
        }

        int carry = 1;
        for (int i = 0; i < 31; i++) {
            int sum = bits[i] + carry;
            bits[i] = sum % 2;
            carry = sum / 2;
            if (carry == 0) break;
        }
    }
}

void BinaryCode::toReverseCode() {
    if (bits[31] == 1) {
        for (int i = 0; i < 31; i++) {
            bits[i] = (bits[i] ? 0 : 1);
        }
    }
}

void BinaryCode::fromAdditionalToDirect() {
    if (bits[31] == 1) {
        int carry = 1;
        for (int i = 0; i < 31; i++) {
            int diff = bits[i] - carry;
            if (diff < 0) {
                bits[i] = 1;
                carry = 1;
            }
            else {
                bits[i] = diff;
                carry = 0;
                break;
            }
        }

        for (int i = 0; i < 31; i++) {
            bits[i] = (bits[i] ? 0 : 1);
        }
    }
}

BinaryCode BinaryCode::addAdditional(const BinaryCode& a, const BinaryCode& b) {
    BinaryCode result;
    int carry = 0;

    for (int i = 0; i < 32; i++) {
        int sum = a.getBit(i) + b.getBit(i) + carry;
        result.setBit(i, sum % 2);
        carry = sum / 2;
    }
    return result;
}

BinaryCode BinaryCode::subtractAdditional(const BinaryCode& a, const BinaryCode& b) {
    BinaryCode minusB;
    minusB.copyFrom(b);

    for (int i = 0; i < 32; i++) {
        minusB.setBit(i, b.getBit(i) ? 0 : 1);
    }

    int carry = 1;
    for (int i = 0; i < 32; i++) {
        int sum = minusB.getBit(i) + carry;
        minusB.setBit(i, sum % 2);
        carry = sum / 2;
    }

    return addAdditional(a, minusB);
}

BinaryCode BinaryCode::binaryAdd(const BinaryCode& a, const BinaryCode& b) {
    BinaryCode result;
    int carry = 0;

    for (int i = 0; i < 32; i++) {
        int sum = a.getBit(i) + b.getBit(i) + carry;
        result.setBit(i, sum % 2);
        carry = sum / 2;
    }
    return result;
}

BinaryCode BinaryCode::binarySubtract(const BinaryCode& a, const BinaryCode& b) {
    BinaryCode result;
    int borrow = 0;

    for (int i = 0; i < 32; i++) {
        int diff = a.getBit(i) - b.getBit(i) - borrow;
        if (diff < 0) {
            diff += 2;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result.setBit(i, diff);
    }
    return result;
}

bool BinaryCode::isGreaterOrEqual(const BinaryCode& a, const BinaryCode& b) {
    // Сначала проверяем знаковые биты
    if (a.getBit(31) != b.getBit(31)) {
        return a.getBit(31) == 0;
    }

    // Если знаки одинаковые, сравниваем остальные биты
    for (int i = 30; i >= 0; i--) {
        if (a.getBit(i) > b.getBit(i)) return true;
        if (a.getBit(i) < b.getBit(i)) return false;
    }
    return true;
}

void BinaryCode::shiftLeft() {
    for (int i = 30; i >= 0; i--) {
        bits[i + 1] = bits[i];
    }
    bits[0] = 0;
}

void BinaryCode::shiftRight() {
    for (int i = 0; i < 30; i++) {
        bits[i] = bits[i + 1];
    }
    bits[30] = 0;
}

BinaryCode BinaryCode::multiplyDirect(const BinaryCode& a, const BinaryCode& b) {
    int signResult = a.getBit(31) ^ b.getBit(31);

    BinaryCode aMag, bMag;
    aMag.copyFrom(a);
    bMag.copyFrom(b);
    aMag.setBit(31, 0);
    bMag.setBit(31, 0);

    BinaryCode result;

    for (int i = 0; i < 31; i++) {
        if (bMag.getBit(i) == 1) {
            BinaryCode shifted;
            shifted.copyFrom(aMag);

            for (int s = 0; s < i; s++) {
                shifted.shiftLeft();
            }

            result = binaryAdd(result, shifted);
        }
    }

    result.setBit(31, signResult);
    return result;
}

BinaryCode BinaryCode::divideRestoring(const BinaryCode& a, const BinaryCode& b,
    double& decimalResult,
    BinaryCode& remainder_out) {

    BinaryCode quotient;
    int signResult = a.getBit(31) ^ b.getBit(31);

    BinaryCode dividend, divisor;
    dividend.copyFrom(a);
    divisor.copyFrom(b);
    dividend.setBit(31, 0);
    divisor.setBit(31, 0);

    // Проверка на ноль
    bool divisorIsZero = true;
    for (int i = 0; i < 31; i++) {
        if (divisor.getBit(i) != 0) {
            divisorIsZero = false;
            break;
        }
    }

    if (divisorIsZero) {
        cout << "Ошибка: деление на ноль!" << endl;
        decimalResult = 0;
        return quotient;
    }

    // Остаток начинается с 0
    BinaryCode remainder;

    cout << "\n=== ДЕЛЕНИЕ С ВОССТАНОВЛЕНИЕМ ОСТАТКА ===\n";
    cout << "Делимое: "; dividend.printBinary();
    cout << " (" << dividend.fromDirectToDecimal() << ")\n";
    cout << "Делитель: "; divisor.printBinary();
    cout << " (" << divisor.fromDirectToDecimal() << ")\n\n";

    // Основной цикл - 31 раз
    for (int i = 0; i < 31; i++) {
        // Сдвигаем остаток влево
        remainder.shiftLeft();

        // Добавляем следующий бит из делимого
        int nextBit = dividend.getBit(30 - i);
        remainder.setBit(0, nextBit);

        // Запоминаем для восстановления
        BinaryCode prevRemainder;
        prevRemainder.copyFrom(remainder);

        // Пробуем вычесть делитель
        BinaryCode trial = binarySubtract(remainder, divisor);

        quotient.shiftLeft();  // сдвигаем частное

        if (trial.getBit(31) == 0) { // результат >= 0
            remainder.copyFrom(trial);
            quotient.setBit(0, 1);
        }
        else { // результат < 0
            remainder.copyFrom(prevRemainder);
            // quotient уже сдвинут, бит 0 уже стоит 0
        }
    }

    remainder_out.copyFrom(remainder);

    // Получаем целую часть
    int intPart = 0;
    for (int i = 30; i >= 0; i--) {
        intPart = intPart * 2 + quotient.getBit(i);
    }

    // Дробная часть - только если остаток не ноль
    double fraction = 0.0;

    if (remainder.fromDirectToDecimal() != 0) {
        double fractionWeight = 0.5;
        BinaryCode tempRemainder;
        tempRemainder.copyFrom(remainder);

        cout << "\n=== ДРОБНАЯ ЧАСТЬ (остаток не ноль) ===\n";
        cout << "Остаток для дроби: "; tempRemainder.printBinary();
        cout << " (" << tempRemainder.fromDirectToDecimal() << ")\n\n";

        int divisorVal = divisor.fromDirectToDecimal();

        for (int i = 0; i < 20; i++) {
            tempRemainder.shiftLeft();

            if (isGreaterOrEqual(tempRemainder, divisor)) {
                tempRemainder = binarySubtract(tempRemainder, divisor);
                fraction += fractionWeight;
            }

            fractionWeight /= 2.0;

            // Если остаток стал ноль - выходим
            if (tempRemainder.fromDirectToDecimal() == 0) {
                cout << "Остаток стал нулевым, дальнейшие итерации не нужны\n";
                break;
            }
        }
    }
    else {
        cout << "\nОстаток равен нулю - дробная часть отсутствует\n";
    }

    // Формируем десятичный результат с правильным знаком
    decimalResult = intPart + fraction;
    if (signResult == 1) {
        decimalResult = -decimalResult;
    }

    // Создаем результат с правильным знаком для возврата
    BinaryCode resultQuotient;
    resultQuotient.copyFrom(quotient);
    resultQuotient.setBit(31, signResult);

    cout << "\nИТОГ:\n";
    cout << "  Целая часть (модуль): " << intPart << "\n";
    cout << "  Дробная часть: " << fraction << "\n";
    cout << "  Знак: " << (signResult == 1 ? "-" : "+") << "\n";
    cout << "  Результат: " << decimalResult << "\n";

    return resultQuotient;
}