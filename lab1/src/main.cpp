//#include <iostream>
//#include <iomanip>
//#include <cmath>
//#include <cstdlib>
//#include <windows.h> 
//#include "BinaryCode.h"
//#include "FloatIEEE754.h"
//#include "BCD8421.h"
//using namespace std;
//
//void waitForEnter() {
//    cout << "\nНажмите Enter для продолжения...";
//    cin.ignore();
//    cin.get();
//}

//int main() {
//    // Установка кодировки для консоли Windows
//    SetConsoleOutputCP(1251); // Используем Windows-1251 вместо UTF-8
//    SetConsoleCP(1251);
//    setlocale(LC_ALL, "Russian");
//
//    int choice;
//
//    do {
//        cout << "\n==============================================================\n";
//        cout << "Лабораторная работа: Изучение представления чисел в памяти компьютера\n";
//        cout << "==============================================================\n\n";
//
//        cout << "Выберите действие:\n";
//        cout << "1 - Целые числа (BinaryCode)\n";
//        cout << "2 - IEEE-754-2008 (числа с плавающей точкой)\n";
//        cout << "3 - 8421 BCD (двоично-десятичный код)\n";
//        cout << "0 - Выход\n\n";
//        cout << "Ваш выбор: ";
//        cin >> choice;
//
//        switch (choice) {
//        case 1: {
//            cout << "\n==============================================================\n";
//            cout << "1. ЦЕЛЫЕ ЧИСЛА (Прямой/Обратный/Дополнительный коды)\n";
//            cout << "==============================================================\n\n";
//
//            int num1, num2;
//            cout << "Введите первое целое число: ";
//            cin >> num1;
//            cout << "Введите второе целое число: ";
//            cin >> num2;
//
//            BinaryCode direct1, direct2;
//            direct1.fromDecimalToDirect(num1);
//            direct2.fromDecimalToDirect(num2);
//
//            cout << "\nЧисло 1 = " << num1 << "\n";
//            cout << "Прямой код: "; direct1.printBinary();
//            cout << " (дес: " << direct1.fromDirectToDecimal() << ")\n";
//
//            BinaryCode rev1;
//            rev1.copyFrom(direct1);
//            rev1.toReverseCode();
//            cout << "Обратный код: "; rev1.printBinary();
//            cout << "\n";
//
//            BinaryCode add1;
//            add1.copyFrom(direct1);
//            add1.toAdditionalCode();
//            cout << "Дополнительный код: "; add1.printBinary();
//            cout << "\n";
//
//            cout << "\nЧисло 2 = " << num2 << "\n";
//            cout << "Прямой код: "; direct2.printBinary();
//            cout << " (дес: " << direct2.fromDirectToDecimal() << ")\n";
//
//            BinaryCode rev2;
//            rev2.copyFrom(direct2);
//            rev2.toReverseCode();
//            cout << "Обратный код: "; rev2.printBinary();
//            cout << "\n";
//
//            BinaryCode add2;
//            add2.copyFrom(direct2);
//            add2.toAdditionalCode();
//            cout << "Дополнительный код: "; add2.printBinary();
//            cout << "\n";
//
//            cout << "\n--- Сложение в дополнительном коде ---\n";
//            BinaryCode sum = BinaryCode::addAdditional(add1, add2);
//            cout << "Результат: "; sum.printBinary();
//
//            BinaryCode sumDirect;
//            sumDirect.copyFrom(sum);
//            sumDirect.fromAdditionalToDirect();
//            cout << " (дес: " << sumDirect.fromDirectToDecimal() << ")\n";
//            cout << "Проверка: " << num1 << " + " << num2 << " = " << num1 + num2 << "\n";
//
//            cout << "\n--- Вычитание (" << num1 << " - " << num2 << ") ---\n";
//            BinaryCode diff = BinaryCode::subtractAdditional(add1, add2);
//            cout << "Результат: "; diff.printBinary();
//
//            BinaryCode diffDirect;
//            diffDirect.copyFrom(diff);
//            diffDirect.fromAdditionalToDirect();
//            cout << " (дес: " << diffDirect.fromDirectToDecimal() << ")\n";
//            cout << "Проверка: " << num1 << " - " << num2 << " = " << num1 - num2 << "\n";
//
//            cout << "\n--- Умножение в прямом коде ---\n";
//            BinaryCode product = BinaryCode::multiplyDirect(direct1, direct2);
//            cout << "Результат: "; product.printBinary();
//            cout << " (дес: " << product.fromDirectToDecimal() << ")\n";
//            cout << "Проверка: " << num1 << " * " << num2 << " = " << num1 * num2 << "\n";
//
//            cout << "\n--- Деление с восстановлением остатка ---\n";
//
//            if (num2 == 0) {
//                cout << "Ошибка: деление на ноль!\n";
//            }
//            else {
//                double divResult;
//                BinaryCode quotient;
//                BinaryCode remainder;
//
//                quotient = BinaryCode::divideRestoring(direct1, direct2, divResult, remainder);
//
//                cout << "Результат деления:\n";
//                cout << "  Частное (целая часть): ";
//                quotient.printBinary();
//                cout << " (десятичное: " << quotient.fromDirectToDecimal() << ")\n";
//
//                cout << "  Остаток: ";
//                remainder.printBinary();
//                cout << " (десятичное: " << remainder.fromDirectToDecimal() << ")\n";
//
//                cout << "  Результат с дробной частью: " << fixed << setprecision(5) << divResult << "\n";
//                cout << "  Проверка: " << num1 << " / " << num2 << " = " << (double)num1 / num2 << "\n";
//            }
//
//            waitForEnter();
//            break;
//        }
//
//        case 2: {
//            cout << "\n==============================================================\n";
//            cout << "2. IEEE-754-2008 (числа с плавающей точкой)\n";
//            cout << "==============================================================\n\n";
//
//            float f1, f2;
//            cout << "Введите первое число: ";
//            cin >> f1;
//            cout << "Введите второе число: ";
//            cin >> f2;
//
//            FloatIEEE754 ieee1, ieee2;
//            ieee1.fromDecimal(f1);
//            ieee2.fromDecimal(f2);
//
//            cout << "\nЧисло " << f1 << ":\n  ";
//            ieee1.printBinary();
//            cout << "\n  (дес: " << ieee1.toDecimal() << ")\n";
//
//            cout << "\nЧисло " << f2 << ":\n  ";
//            ieee2.printBinary();
//            cout << "\n  (дес: " << ieee2.toDecimal() << ")\n";
//
//            FloatIEEE754 sumFloat = FloatIEEE754::add(ieee1, ieee2);
//            cout << "\nСложение:  "; sumFloat.printBinary();
//            cout << " (дес: " << sumFloat.toDecimal() << ")\n";
//            cout << "Проверка:  " << f1 << " + " << f2 << " = " << f1 + f2 << "\n";
//
//            FloatIEEE754 diffFloat = FloatIEEE754::subtract(ieee1, ieee2);
//            cout << "\nВычитание: "; diffFloat.printBinary();
//            cout << " (дес: " << diffFloat.toDecimal() << ")\n";
//            cout << "Проверка:  " << f1 << " - " << f2 << " = " << f1 - f2 << "\n";
//
//            FloatIEEE754 multFloat = FloatIEEE754::multiply(ieee1, ieee2);
//            cout << "\nУмножение: "; multFloat.printBinary();
//            cout << " (дес: " << multFloat.toDecimal() << ")\n";
//            cout << "Проверка:  " << f1 << " * " << f2 << " = " << f1 * f2 << "\n";
//
//            if (f2 != 0) {
//                FloatIEEE754 divFloat = FloatIEEE754::divide(ieee1, ieee2);
//                cout << "\nДеление:   "; divFloat.printBinary();
//                cout << " (дес: " << divFloat.toDecimal() << ")\n";
//                cout << "Проверка:  " << f1 << " / " << f2 << " = " << f1 / f2 << "\n";
//            }
//            else {
//                cout << "\nДеление на ноль!\n";
//            }
//
//            waitForEnter();
//            break;
//        }
//
//        case 3: {
//            cout << "\n==============================================================\n";
//            cout << "3. 8421 BCD (двоично-десятичный код)\n";
//            cout << "==============================================================\n\n";
//
//            int bcdNum1, bcdNum2;
//            cout << "Введите первое число: ";
//            cin >> bcdNum1;
//            cout << "Введите второе число: ";
//            cin >> bcdNum2;
//
//            BCD8421 bcd1, bcd2;
//            bcd1.fromDecimal(bcdNum1);
//            bcd2.fromDecimal(bcdNum2);
//
//            cout << "\nЧисло " << bcdNum1 << " в BCD:\n  ";
//            bcd1.printBCD();
//            cout << "\n  (дес: " << bcd1.toDecimal() << ")\n";
//
//            cout << "\nЧисло " << bcdNum2 << " в BCD:\n  ";
//            bcd2.printBCD();
//            cout << "\n  (дес: " << bcd2.toDecimal() << ")\n";
//
//            cout << "\n--- ОПЕРАЦИИ В BCD ---\n";
//
//            BCD8421 bcdSum = BCD8421::add(bcd1, bcd2);
//            cout << "\nСложение:  "; bcdSum.printBCD();
//            cout << " (дес: " << bcdSum.toDecimal() << ")\n";
//            cout << "Проверка:  " << bcdNum1 << " + " << bcdNum2 << " = " << bcdNum1 + bcdNum2 << "\n";
//
//            waitForEnter();
//            break;
//        }
//
//        case 0:
//            cout << "\nВыход из программы...\n";
//            break;
//
//        default:
//            cout << "\nОшибка: неверный выбор! Попробуйте снова.\n";
//            waitForEnter();
//            break;
//        }
//
//    } while (choice != 0);
//
//    return 0;
//}