#include "BCD8421.h"
#include <cmath>
#include <iostream>
using namespace std;

// Конструктор
BCD8421::BCD8421() {
    for (int i = 0; i < 8; i++) digits[i] = 0;
}


unsigned char BCD8421::toBCD(unsigned char value) {
    return value & 0x0F;
}

unsigned char BCD8421::fromBCD(unsigned char bcd) {
    return bcd & 0x0F;
}

// BCD коррекция: если цифра >= 10 или есть перенос, добавляем 6
void BCD8421::correctDigit(unsigned char& digit, int& carry) {
    if (digit >= 10 || carry == 1) {
        digit = (digit + 6) & 0x0F;
        carry = 1;
    }
    else {
        carry = 0;
    }
}



void BCD8421::toAdditionalCode() {
    // Проверяем знаковую тетраду (digits[0])
    if ((digits[0] & 0x0F) == 0x0F) { // 1111 = минус
        // 1. Инверсия по цифрам (дополнение до 9)
        for (int i = 1; i < 8; i++) { // со 2-й по 8-ю тетраду
            int val = fromBCD(digits[i]);
            val = 9 - val;  // дополнение до 9
            digits[i] = toBCD(val);
        }

        // 2. Прибавляем 1 к младшему разряду (доп. код до 10)
        int carry = 1;
        for (int i = 7; i >= 1; i--) {
            int val = fromBCD(digits[i]) + carry;
            if (val >= 10) {
                digits[i] = toBCD(val - 10);
                carry = 1;
            }
            else {
                digits[i] = toBCD(val);
                carry = 0;
                break;
            }
        }
    }
    // Знаковая тетрада остается 1111 для отрицательных
}

void BCD8421::fromAdditionalToDirect() {
    // Проверяем знаковую тетраду (digits[0])
    if ((digits[0] & 0x0F) == 0x0F) { // 1111 = минус
        // 1. Вычитаем 1 из младшего разряда
        int borrow = 1;
        for (int i = 7; i >= 1; i--) {
            int val = fromBCD(digits[i]) - borrow;
            if (val < 0) {
                digits[i] = toBCD(val + 10);
                borrow = 1;
            }
            else {
                digits[i] = toBCD(val);
                borrow = 0;
                break;
            }
        }

        // 2. Инверсия по цифрам (обратно из доп. до 9)
        for (int i = 1; i < 8; i++) {
            int val = fromBCD(digits[i]);
            val = 9 - val;  // обратно из дополнения до 9
            digits[i] = toBCD(val);
        }
    }
    // Знаковая тетрада остается 1111
}


void BCD8421::fromDecimal(int num) {
    // Очищаем
    for (int i = 0; i < 8; i++) digits[i] = 0;

    if (num == 0) {
        digits[0] = toBCD(0); // знаковая тетрада = 0 (плюс)
        return;
    }

    // Определяем знак
    if (num < 0) {
        digits[0] = toBCD(0x0F); // 1111 = минус
        num = -num;
    }
    else {
        digits[0] = toBCD(0); // 0000 = плюс
    }

    // Записываем цифры справа налево (с 7-й по 1-ю)
    int index = 7;
    while (num > 0 && index >= 1) {
        int digit = num % 10;
        digits[index] = toBCD(digit);
        num /= 10;
        index--;
    }

    // Если число отрицательное, преобразуем в дополнительный код
    if ((digits[0] & 0x0F) == 0x0F) {
        toAdditionalCode();
    }
}



int BCD8421::toDecimal() const {
    BCD8421 temp = *this;

    // Если число отрицательное, преобразуем из дополнительного кода
    if ((temp.digits[0] & 0x0F) == 0x0F) {
        temp.fromAdditionalToDirect();
    }

    // Собираем число
    int result = 0;
    for (int i = 1; i < 8; i++) {
        result = result * 10 + fromBCD(temp.digits[i]);
    }

    // Применяем знак
    return ((digits[0] & 0x0F) == 0x0F) ? -result : result;
}


void BCD8421::printBCD() const {
    for (int i = 0; i < 8; i++) {
        for (int j = 3; j >= 0; j--) {
            cout << ((digits[i] >> j) & 1);
        }
        if (i < 7) cout << " ";
    }
    cout << " (";
    if ((digits[0] & 0x0F) == 0x0F) cout << "-"; else cout << "+";
    cout << toDecimal() << ")";
}


BCD8421 BCD8421::add(const BCD8421& a, const BCD8421& b) {
    BCD8421 result;

    cout << "\n=== BCD СЛОЖЕНИЕ В ДОП. КОДЕ ===\n";
    cout << "  a: "; a.printBCD(); cout << "\n";
    cout << "  b: "; b.printBCD(); cout << "\n";
    cout << "----------------\n";

    int carry = 0; // перенос между тетрадами

    // Складываем справа налево (от младших тетрад к старшим)
    for (int i = 7; i >= 0; i--) {
        // Складываем как обычные двоичные числа
        int aVal = fromBCD(a.digits[i]);
        int bVal = fromBCD(b.digits[i]);
        int sum = aVal + bVal + carry;

        cout << "Тетрада " << i << ": " << aVal << " + " << bVal
            << " + перенос " << carry << " = " << sum;

        // BCD коррекция
        if (sum >= 10) {
            result.digits[i] = toBCD(sum - 10);
            carry = 1;
            cout << " → " << (sum - 10) << ", перенос=1";
        }
        else {
            result.digits[i] = toBCD(sum);
            carry = 0;
            cout << " → " << sum << ", перенос=0";
        }
        cout << "\n";
    }

    cout << "----------------\n";
    cout << "Результат: "; result.printBCD(); cout << "\n";

    return result;

}
