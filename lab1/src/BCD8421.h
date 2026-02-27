#ifndef BCD8421_H
#define BCD8421_H

#include <iostream>
using namespace std;

class BCD8421 {
private:
    unsigned char digits[8];  // 8 цифр (каждая 4 бита)
    // digits[0] - самая левая тетрада (знаковая!)

public:
    BCD8421();

    // Преобразование
    void fromDecimal(int num);
    int toDecimal() const;
    void printBCD() const;

    // Арифметика в дополнительном коде
    static BCD8421 add(const BCD8421& a, const BCD8421& b);

private:
    // Вспомогательные методы
    static unsigned char toBCD(unsigned char value);
    static unsigned char fromBCD(unsigned char bcd);

    // Преобразование в дополнительный код и обратно
    void toAdditionalCode();      // в доп. код (для отрицательных)
    void fromAdditionalToDirect(); // из доп. кода обратно

    // BCD коррекция
    static void correctDigit(unsigned char& digit, int& carry);
};

#endif