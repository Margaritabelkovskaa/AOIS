#include "FloatIEEE754.h"
#include <cmath>
#include <iostream>
using namespace std;

// Конструктор
FloatIEEE754::FloatIEEE754() {
    for (int i = 0; i < 32; i++) bits[i] = 0;
}

// Преобразование из десятичного
void FloatIEEE754::fromDecimal(float num) {
    union {
        float f;
        unsigned int i;
    } u;
    u.f = num;

    for (int i = 0; i < 32; i++) {
        bits[i] = (u.i >> i) & 1;
    }
}

// Преобразование в десятичное
float FloatIEEE754::toDecimal() const {
    union {
        float f;
        unsigned int i;
    } u;

    u.i = 0;
    for (int i = 0; i < 32; i++) {
        if (bits[i]) {
            u.i |= (1 << i);
        }
    }
    return u.f;
}

// Печать двоичного представления
void FloatIEEE754::printBinary() const {
    for (int i = 31; i >= 0; i--) {
        cout << bits[i];
        if (i == 31 || i == 23) cout << " ";
    }
}

// ============ МЕТОДЫ ДОСТУПА К КОМПОНЕНТАМ ============

int FloatIEEE754::getExponent() const {
    int exp = 0;
    for (int i = 0; i < 8; i++) {
        if (bits[23 + i]) {
            exp |= (1 << i);
        }
    }
    return exp;
}

unsigned int FloatIEEE754::getMantissa() const {
    unsigned int mant = 0;
    for (int i = 0; i < 23; i++) {
        if (bits[i]) {
            mant |= (1 << i);
        }
    }
    return mant;
}

void FloatIEEE754::setExponent(int e) {
    for (int i = 0; i < 8; i++) {
        bits[23 + i] = (e >> i) & 1;
    }
}

void FloatIEEE754::setMantissa(unsigned int m) {
    for (int i = 0; i < 23; i++) {
        bits[i] = (m >> i) & 1;
    }
}

// ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ============

// Нормализация мантиссы
void normalize(unsigned int& mant, int& exp) {
    if (mant == 0) {
        exp = 0;
        return;
    }
    // Пока нет единицы в 24-й позиции (счет от 0)
    while ((mant & (1 << 23)) == 0) {
        mant <<= 1;
        exp--;
    }
}

// Проверка специальных случаев
bool isNaN(int exp, unsigned int mant) {
    return (exp == 255 && mant != 0);
}

bool isInfinity(int exp, unsigned int mant) {
    return (exp == 255 && mant == 0);
}

bool isZero(int exp, unsigned int mant) {
    return (exp == 0 && mant == 0);
}

// ============ АППАРАТНОЕ СЛОЖЕНИЕ ============

FloatIEEE754 FloatIEEE754::add(const FloatIEEE754& a, const FloatIEEE754& b) {
    // Извлекаем компоненты
    int signA = a.getSign();
    int signB = b.getSign();
    int expA = a.getExponent();
    int expB = b.getExponent();
    unsigned int mantA = a.getMantissa();
    unsigned int mantB = b.getMantissa();

    // ПРОВЕРКА СПЕЦИАЛЬНЫХ СЛУЧАЕВ
    // NaN
    if (isNaN(expA, mantA)) return a;
    if (isNaN(expB, mantB)) return b;

    // Бесконечности
    if (isInfinity(expA, mantA)) {
        if (isInfinity(expB, mantB)) {
           
            if (signA == signB) {
                FloatIEEE754 res;
                res.setSign(signA);
                res.setExponent(255);
                res.setMantissa(0);
                return res;
            }
            else {
                // ∞ + (-∞) = NaN
                FloatIEEE754 res;
                res.setExponent(255);
                res.setMantissa(1); // любое ненулевое значение
                return res;
            }
        }
        return a; // ∞ + X = ∞
    }
    if (isInfinity(expB, mantB)) return b;

    // Добавляем неявную единицу
    if (expA != 0) mantA |= (1 << 23);
    if (expB != 0) mantB |= (1 << 23);

    int expRes = expA;
    unsigned int mantRes;
    int signRes;

    // ВЫРАВНИВАНИЕ ЭКСПОНЕНТ
    if (expA > expB) {
        int shift = expA - expB;
        if (shift < 32) {
            mantB >>= shift;
        }
        else {
            mantB = 0;
        }
        expRes = expA;
    }
    else if (expB > expA) {
        int shift = expB - expA;
        if (shift < 32) {
            mantA >>= shift;
        }
        else {
            mantA = 0;
        }
        expRes = expB;
    }

    // СЛОЖЕНИЕ/ВЫЧИТАНИЕ МАНТИСС
    if (signA == signB) {
        // Одинаковые знаки - складываем
        mantRes = mantA + mantB;
        signRes = signA;

        // Обработка переполнения
        if (mantRes & (1 << 24)) {
            mantRes >>= 1;
            expRes++;
        }
    }
    else {
        // Разные знаки - вычитаем
        if (mantA >= mantB) {
            mantRes = mantA - mantB;
            signRes = signA;
        }
        else {
            mantRes = mantB - mantA;
            signRes = signB;
        }

        // Нормализация
        if (mantRes != 0) {
            while ((mantRes & (1 << 23)) == 0) {
                mantRes <<= 1;
                expRes--;
            }
        }
    }

    // ПРОВЕРКА НА ПЕРЕПОЛНЕНИЕ ЭКСПОНЕНТЫ
    if (expRes >= 255) {
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(255);
        res.setMantissa(0);
        return res; // бесконечность
    }

    // ПРОВЕРКА НА ИСЧЕЗНОВЕНИЕ ПОРЯДКА
    if (expRes <= 0) {
        if (expRes <= -24 || mantRes == 0) {
            FloatIEEE754 res;
            res.setSign(signRes);
            return res; // ноль
        }
        // Денормализованное число
        mantRes >>= (1 - expRes);
        expRes = 0;
    }

    // Убираем неявную единицу
    if (expRes != 0) {
        mantRes &= ~(1 << 23);
    }
    else {
        // Для денормализованных неявной единицы нет
    }

    // СБОРКА РЕЗУЛЬТАТА
    FloatIEEE754 result;
    result.setSign(signRes);
    result.setExponent(expRes);
    result.setMantissa(mantRes);
    return result;
}

// ============ АППАРАТНОЕ ВЫЧИТАНИЕ ============

FloatIEEE754 FloatIEEE754::subtract(const FloatIEEE754& a, const FloatIEEE754& b) {
    // a - b = a + (-b)
    FloatIEEE754 negB = b;
    negB.setSign(!b.getSign());
    return add(a, negB);
}

// ============ АППАРАТНОЕ УМНОЖЕНИЕ ============

// ============ ИСПРАВЛЕННОЕ АППАРАТНОЕ УМНОЖЕНИЕ ============
FloatIEEE754 FloatIEEE754::multiply(const FloatIEEE754& a, const FloatIEEE754& b) {
    // Извлекаем компоненты
    int signA = a.getSign();
    int signB = b.getSign();
    int expA = a.getExponent();
    int expB = b.getExponent();
    unsigned int mantA = a.getMantissa();
    unsigned int mantB = b.getMantissa();

    // Знак результата
    int signRes = signA ^ signB;

    // ПРОВЕРКА СПЕЦИАЛЬНЫХ СЛУЧАЕВ
    if (isNaN(expA, mantA)) return a;
    if (isNaN(expB, mantB)) return b;

    if (isInfinity(expA, mantA)) {
        if (isZero(expB, mantB)) {
            FloatIEEE754 res;
            res.setExponent(255);
            res.setMantissa(1);
            return res; // NaN
        }
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(255);
        return res;
    }
    if (isInfinity(expB, mantB)) {
        if (isZero(expA, mantA)) {
            FloatIEEE754 res;
            res.setExponent(255);
            res.setMantissa(1);
            return res; // NaN
        }
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(255);
        return res;
    }

    if (isZero(expA, mantA) || isZero(expB, mantB)) {
        FloatIEEE754 res;
        res.setSign(signRes);
        return res;
    }

    // ИСПРАВЛЕНО: Правильные реальные экспоненты
    int realExpA = (expA == 0) ? -126 : expA - 127;
    int realExpB = (expB == 0) ? -126 : expB - 127;

    // Добавляем неявную единицу
    unsigned long long mantLongA = mantA;
    unsigned long long mantLongB = mantB;

    if (expA != 0) mantLongA |= (1ULL << 23);
    if (expB != 0) mantLongB |= (1ULL << 23);

    // ИСПРАВЛЕНО: Умножение 24-битных чисел
    unsigned long long mantRes = mantLongA * mantLongB;

    // ИСПРАВЛЕНО: Вычисление экспоненты
    int expRes = realExpA + realExpB + 127;

    // ИСПРАВЛЕНО: Правильная нормализация
    if (mantRes & (1ULL << 47)) {
        // Результат в [2, 4)
        mantRes >>= 24;  // Сдвигаем на 24, чтобы получить 24 бита
        expRes++;
    }
    else {
        // Результат в [1, 2)
        mantRes >>= 23;  // Сдвигаем на 23, чтобы получить 24 бита
    }

    // Проверка на переполнение
    if (expRes >= 255) {
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(255);
        return res;
    }

    // Проверка на исчезновение порядка
    if (expRes <= 0) {
        if (expRes <= -24) {
            FloatIEEE754 res;
            res.setSign(signRes);
            return res;
        }
        mantRes >>= (1 - expRes);
        expRes = 0;
    }

    // Убираем неявную единицу
    unsigned int finalMant;
    if (expRes != 0) {
        finalMant = mantRes & ~(1ULL << 23);
    }
    else {
        finalMant = (unsigned int)mantRes;
    }

    FloatIEEE754 result;
    result.setSign(signRes);
    result.setExponent(expRes);
    result.setMantissa(finalMant);
    return result;
}// ============ АППАРАТНОЕ ДЕЛЕНИЕ ============

// ИСПРАВЛЕННОЕ ДЕЛЕНИЕ:

// ============ ИСПРАВЛЕННОЕ АППАРАТНОЕ ДЕЛЕНИЕ ============

// ============ АППАРАТНОЕ ДЕЛЕНИЕ ============

FloatIEEE754 FloatIEEE754::divide(const FloatIEEE754& a, const FloatIEEE754& b) {
    // Извлекаем компоненты
    int signA = a.getSign();
    int signB = b.getSign();
    int expA = a.getExponent();
    int expB = b.getExponent();
    unsigned int mantA = a.getMantissa();
    unsigned int mantB = b.getMantissa();

    // Знак результата
    int signRes = signA ^ signB;

    // ПРОВЕРКА СПЕЦИАЛЬНЫХ СЛУЧАЕВ
    if (isNaN(expA, mantA)) return a;
    if (isNaN(expB, mantB)) return b;

    // Проверка на деление на ноль
    if (isZero(expB, mantB)) {
        if (isZero(expA, mantA)) {
            // 0 / 0 = NaN
            FloatIEEE754 res;
            res.setExponent(255);
            res.setMantissa(1);
            return res;
        }
        // X / 0 = Infinity
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(255);
        res.setMantissa(0);
        return res;
    }

    // Бесконечности
    if (isInfinity(expA, mantA)) {
        if (isInfinity(expB, mantB)) {
            // ∞ / ∞ = NaN
            FloatIEEE754 res;
            res.setExponent(255);
            res.setMantissa(1);
            return res;
        }
        // ∞ / X = ∞
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(255);
        res.setMantissa(0);
        return res;
    }

    if (isInfinity(expB, mantB)) {
        // X / ∞ = 0
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(0);
        res.setMantissa(0);
        return res;
    }

    if (isZero(expA, mantA)) {
        // 0 / X = 0
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(0);
        res.setMantissa(0);
        return res;
    }

    // Получаем реальные экспоненты (со смещением 127)
    int realExpA = (expA == 0) ? -126 : expA - 127;
    int realExpB = (expB == 0) ? -126 : expB - 127;

    // Добавляем неявную единицу к мантиссам (24 бита: 1 + 23)
    unsigned long long mantLongA = mantA;
    unsigned long long mantLongB = mantB;

    if (expA != 0) mantLongA |= (1ULL << 23);
    if (expB != 0) mantLongB |= (1ULL << 23);

    // ============ ПРАВИЛЬНОЕ ДЕЛЕНИЕ ============

    // ШАГ 1: Нормализуем делимое, чтобы оно было не меньше делителя
    int shift = 0;
    unsigned long long dividend = mantLongA;
    unsigned long long divisor = mantLongB;

    while (dividend < divisor && shift < 32) {
        dividend <<= 1;
        shift++;
    }

    // ШАГ 2: Выполняем деление для получения 24 бит мантиссы
    unsigned long long quotient = 0;
    unsigned long long remainder = dividend;

    for (int i = 0; i < 24; i++) {
        quotient <<= 1;
        if (remainder >= divisor) {
            quotient |= 1;
            remainder -= divisor;
        }
        remainder <<= 1;
    }

    // ШАГ 3: Убираем лишние сдвиги из частного
    if (shift > 0) {
        quotient >>= shift;
    }

    // ШАГ 4: Вычисляем экспоненту результата
    int expRes = realExpA - realExpB + 127;

    // ШАГ 5: Нормализация результата (приводим к виду 1.xxx)
    if (quotient & (1ULL << 24)) {
        // Частное >= 2, сдвигаем вправо и увеличиваем экспоненту
        quotient >>= 1;
        expRes++;
    }
    else if ((quotient & (1ULL << 23)) == 0) {
        // Частное < 1, сдвигаем влево пока не получим 1.xxx
        int normShift = 0;
        while ((quotient & (1ULL << 23)) == 0 && quotient != 0) {
            quotient <<= 1;
            normShift++;
        }
        expRes -= normShift;
    }

    // ШАГ 6: Проверка на переполнение экспоненты
    if (expRes >= 255) {
        FloatIEEE754 res;
        res.setSign(signRes);
        res.setExponent(255);
        res.setMantissa(0);
        return res; // бесконечность
    }

    // ШАГ 7: Проверка на исчезновение порядка (underflow)
    if (expRes <= 0) {
        if (expRes <= -24 || quotient == 0) {
            // Слишком маленькое число - возвращаем 0
            FloatIEEE754 res;
            res.setSign(signRes);
            res.setExponent(0);
            res.setMantissa(0);
            return res;
        }

        // Денормализованное число
        quotient >>= (1 - expRes);
        expRes = 0;
    }

    // ШАГ 8: Оставляем только 24 бита мантиссы
    quotient &= ((1ULL << 24) - 1);

    // ШАГ 9: Убираем неявную единицу
    unsigned int finalMant;
    if (expRes != 0) {
        finalMant = (unsigned int)(quotient & ~(1ULL << 23));
    }
    else {
        finalMant = (unsigned int)quotient;
    }

    // ШАГ 10: Сборка результата
    FloatIEEE754 result;
    result.setSign(signRes);
    result.setExponent(expRes);
    result.setMantissa(finalMant);

    return result;
}