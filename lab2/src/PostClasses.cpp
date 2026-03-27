#include "PostClasses.h"
#include <iostream>
#include <cmath>

using namespace std;

PostClasses::PostClasses(const TruthTable& tt) : truth_table(tt) {}

void PostClasses::checkAll() {
    cout << "\n=== Классы Поста ===\n";
    cout << "T0 (сохраняет 0): " << (isT0() ? "Да" : "Нет") << "\n";
    cout << "T1 (сохраняет 1): " << (isT1() ? "Да" : "Нет") << "\n";
    cout << "S (самодвойственная): " << (isS() ? "Да" : "Нет") << "\n";
    cout << "M (монотонная): " << (isM() ? "Да" : "Нет") << "\n";
    cout << "L (линейная): " << (isL() ? "Да" : "Нет") << "\n";
}

bool PostClasses::isT0() const {
    return truth_table.getTable()[0].back() == 0;
}

bool PostClasses::isT1() const {
    int rows = truth_table.getTable().size();
    return truth_table.getTable()[rows - 1].back() == 1;
}

bool PostClasses::isS() const {
    int rows = truth_table.getTable().size();
    for (int i = 0; i < rows / 2; i++) {
        if (truth_table.getTable()[i].back() == truth_table.getTable()[rows - 1 - i].back()) {
            return false;
        }
    }
    return true;
}

bool PostClasses::isM() const {
    int n = truth_table.getVariables().size();
    int rows = truth_table.getTable().size();

    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < rows; j++) {
            bool dominates = true;
            for (int k = 0; k < n; k++) {
                if (truth_table.getTable()[i][k] > truth_table.getTable()[j][k]) {
                    dominates = false;
                    break;
                }
            }
            if (dominates && truth_table.getTable()[i].back() > truth_table.getTable()[j].back()) {
                return false;
            }
        }
    }
    return true;
}

bool PostClasses::isL() const {
    int n = truth_table.getVariables().size();
    int rows = pow(2, n);
    if (rows == 0) return true;

    vector<int> values(rows);
    for (int i = 0; i < rows; i++) {
        values[i] = truth_table.getTable()[i].back();
    }

    vector<int> coeffs = values;
    for (int i = 0; i < rows; i++) {
        for (int j = rows - 1; j > i; j--) {
            coeffs[j] ^= coeffs[j - 1];
        }
    }

    for (int i = 0; i < rows; i++) {
        if (coeffs[i] == 1) {
            int ones = 0;
            for (int j = 0; j < n; j++) {
                if (i & (1 << (n - j - 1))) {
                    ones++;
                }
            }
            if (ones > 1 && i != 0) {
                return false;
            }
        }
    }
    return true;
}