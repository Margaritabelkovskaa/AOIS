#include "BooleanDerivative.h"
#include <iostream>
#include <cmath>

using namespace std;

BooleanDerivative::BooleanDerivative(const TruthTable& tt) : truth_table(tt) {}

void BooleanDerivative::computeAll() const {
    int n = truth_table.getVariables().size();
    if (n == 0) {
        cout << "Нет переменных для дифференцирования\n";
        return;
    }

    cout << "Частные производные:\n";

    for (int var = 0; var < n; var++) {
        cout << "df/d" << truth_table.getVariables()[var] << ": ";

        int other_count = n - 1;
        int other_rows = pow(2, other_count);

        for (int i = 0; i < other_rows; i++) {
            int idx0 = 0, idx1 = 0;
            int bit_pos = 0;
            for (int k = 0; k < n; k++) {
                if (k == var) {
                    idx0 = (idx0 << 1) | 0;
                    idx1 = (idx1 << 1) | 1;
                }
                else {
                    int val = (i >> (other_count - bit_pos - 1)) & 1;
                    idx0 = (idx0 << 1) | val;
                    idx1 = (idx1 << 1) | val;
                    bit_pos++;
                }
            }
            int diff = truth_table.getTable()[idx0].back() ^ truth_table.getTable()[idx1].back();
            cout << diff;
        }
        cout << "\n";
    }
}