#include <iostream>
#include <iomanip>
#include "BinaryCode.h"
#include "FloatIEEE754.h"
#include "BCD8421.h"
using namespace std;

int main() {
   
    int choice;
     setlocale(LC_ALL, "Russian");
    do {
        cout << "\n==============================================================\n";
        cout << "LABORATORY WORK: Number Representation in Computer Memory\n";
        cout << "==============================================================\n\n";

        cout << "Select an option:\n";
        cout << "1 - Integers (BinaryCode)\n";
        cout << "2 - IEEE-754-2008 (Floating point numbers)\n";
        cout << "3 - 8421 BCD (Binary-coded decimal)\n";
        cout << "0 - Exit\n\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "\n==============================================================\n";
            cout << "1. INTEGERS (Direct/Reverse/Two's complement codes)\n";
            cout << "==============================================================\n\n";

            int num1, num2;
            cout << "Enter first integer: ";
            cin >> num1;
            cout << "Enter second integer: ";
            cin >> num2;

            BinaryCode direct1, direct2;
            direct1.fromDecimalToDirect(num1);
            direct2.fromDecimalToDirect(num2);

            cout << "\nNumber 1 = " << num1 << "\n";
            cout << "Direct code: "; direct1.printBinary();
            cout << " (dec: " << direct1.fromDirectToDecimal() << ")\n";

            BinaryCode rev1;
            rev1.copyFrom(direct1);
            rev1.toReverseCode();
            cout << "Reverse code: "; rev1.printBinary();
            cout << "\n";

            BinaryCode add1;
            add1.copyFrom(direct1);
            add1.toAdditionalCode();
            cout << "Two's complement: "; add1.printBinary();
            cout << "\n";

            cout << "\nNumber 2 = " << num2 << "\n";
            cout << "Direct code: "; direct2.printBinary();
            cout << " (dec: " << direct2.fromDirectToDecimal() << ")\n";

            BinaryCode rev2;
            rev2.copyFrom(direct2);
            rev2.toReverseCode();
            cout << "Reverse code: "; rev2.printBinary();
            cout << "\n";

            BinaryCode add2;
            add2.copyFrom(direct2);
            add2.toAdditionalCode();
            cout << "Two's complement: "; add2.printBinary();
            cout << "\n";

            cout << "\n--- Addition in two's complement ---\n";
            BinaryCode sum = BinaryCode::addAdditional(add1, add2);
            cout << "Result: "; sum.printBinary();

            BinaryCode sumDirect;
            sumDirect.copyFrom(sum);
            sumDirect.fromAdditionalToDirect();
            cout << " (dec: " << sumDirect.fromDirectToDecimal() << ")\n";
            cout << "Check: " << num1 << " + " << num2 << " = " << num1 + num2 << "\n";

            cout << "\n--- Subtraction (" << num1 << " - " << num2 << ") ---\n";
            BinaryCode diff = BinaryCode::subtractAdditional(add1, add2);
            cout << "Result: "; diff.printBinary();

            BinaryCode diffDirect;
            diffDirect.copyFrom(diff);
            diffDirect.fromAdditionalToDirect();
            cout << " (dec: " << diffDirect.fromDirectToDecimal() << ")\n";
            cout << "Check: " << num1 << " - " << num2 << " = " << num1 - num2 << "\n";

            cout << "\n--- Multiplication in direct code ---\n";
            BinaryCode product = BinaryCode::multiplyDirect(direct1, direct2);
            cout << "Result: "; product.printBinary();
            cout << " (dec: " << product.fromDirectToDecimal() << ")\n";
            cout << "Check: " << num1 << " * " << num2 << " = " << num1 * num2 << "\n";

            cout << "\n--- Division with restoring remainder ---\n";

            if (num2 == 0) {
                cout << "Error: division by zero!\n";
            }
            else {
                double divResult;
                BinaryCode quotient;
                BinaryCode remainder;

                quotient = BinaryCode::divideRestoring(direct1, direct2, divResult, remainder);

                cout << "Division result:\n";
                cout << "  Quotient (integer part): ";
                quotient.printBinary();
                cout << " (decimal: " << quotient.fromDirectToDecimal() << ")\n";

                cout << "  Remainder: ";
                remainder.printBinary();
                cout << " (decimal: " << remainder.fromDirectToDecimal() << ")\n";

                cout << "  Result with fractional part: " << fixed << setprecision(5) << divResult << "\n";
                cout << "  Check: " << num1 << " / " << num2 << " = " << (double)num1 / num2 << "\n";
            }
            break;
        }

        case 2: {
            cout << "\n==============================================================\n";
            cout << "2. IEEE-754-2008 (Floating point numbers)\n";
            cout << "==============================================================\n\n";

            float f1, f2;
            cout << "Enter first number: ";
            cin >> f1;
            cout << "Enter second number: ";
            cin >> f2;

            FloatIEEE754 ieee1, ieee2;
            ieee1.fromDecimal(f1);
            ieee2.fromDecimal(f2);

            cout << "\nNumber " << f1 << ":\n  ";
            ieee1.printBinary();
            cout << "\n  (dec: " << ieee1.toDecimal() << ")\n";

            cout << "\nNumber " << f2 << ":\n  ";
            ieee2.printBinary();
            cout << "\n  (dec: " << ieee2.toDecimal() << ")\n";

            FloatIEEE754 sumFloat = FloatIEEE754::add(ieee1, ieee2);
            cout << "\nAddition:  "; sumFloat.printBinary();
            cout << " (dec: " << sumFloat.toDecimal() << ")\n";
            cout << "Check:  " << f1 << " + " << f2 << " = " << f1 + f2 << "\n";

            FloatIEEE754 diffFloat = FloatIEEE754::subtract(ieee1, ieee2);
            cout << "\nSubtraction: "; diffFloat.printBinary();
            cout << " (dec: " << diffFloat.toDecimal() << ")\n";
            cout << "Check:  " << f1 << " - " << f2 << " = " << f1 - f2 << "\n";

            FloatIEEE754 multFloat = FloatIEEE754::multiply(ieee1, ieee2);
            cout << "\nMultiplication: "; multFloat.printBinary();
            cout << " (dec: " << multFloat.toDecimal() << ")\n";
            cout << "Check:  " << f1 << " * " << f2 << " = " << f1 * f2 << "\n";

            if (f2 != 0) {
                FloatIEEE754 divFloat = FloatIEEE754::divide(ieee1, ieee2);
                cout << "\nDivision:   "; divFloat.printBinary();
                cout << " (dec: " << divFloat.toDecimal() << ")\n";
                cout << "Check:  " << f1 << " / " << f2 << " = " << f1 / f2 << "\n";
            }
            else {
                cout << "\nDivision by zero!\n";
            }
            break;
        }

        case 3: {
            cout << "\n==============================================================\n";
            cout << "3. 8421 BCD (Binary-coded decimal)\n";
            cout << "==============================================================\n\n";

            int bcdNum1, bcdNum2;
            cout << "Enter first number: ";
            cin >> bcdNum1;
            cout << "Enter second number: ";
            cin >> bcdNum2;

            BCD8421 bcd1, bcd2;
            bcd1.fromDecimal(bcdNum1);
            bcd2.fromDecimal(bcdNum2);

            cout << "\nNumber " << bcdNum1 << " in BCD:\n  ";
            bcd1.printBCD();
            cout << "\n  (dec: " << bcd1.toDecimal() << ")\n";

            cout << "\nNumber " << bcdNum2 << " in BCD:\n  ";
            bcd2.printBCD();
            cout << "\n  (dec: " << bcd2.toDecimal() << ")\n";

            cout << "\n--- BCD HARDWARE OPERATIONS ---\n";

            // Используем аппаратное сложение вместо простого
            BCD8421 bcdSum = BCD8421::hardwareAdd(bcd1, bcd2);

            cout << "\nHardware Addition Result:  ";
            bcdSum.printBCD();
            cout << "\n";
            cout << "Check: " << bcdNum1 << " + " << bcdNum2 << " = " << bcdNum1 + bcdNum2 << "\n";
            break;
        }
        case 0:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "\nError: invalid choice! Try again.\n";
            break;
        }

    } while (choice != 0);

    return 0;
}
