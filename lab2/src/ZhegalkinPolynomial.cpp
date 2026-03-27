#include "ZhegalkinPolynomial.h"
#include <cmath>

using namespace std;

ZhegalkinPolynomial::ZhegalkinPolynomial(const TruthTable& tt) : truth_table(tt) {}

string ZhegalkinPolynomial::getPolynomial() const {
    int n = truth_table.getVariables().size();
    int rows = pow(2, n);
    if (rows == 0) return "0";

    vector<int> values(rows);
    for (int i = 0; i < rows; i++) {
        values[i] = truth_table.getTable()[i].back();
    }

    for (int i = 0; i < rows; i++) {
        for (int j = rows - 1; j > i; j--) {
            values[j] ^= values[j - 1];
        }
    }

    string result;
    for (int i = 0; i < rows; i++) {
        if (values[i] == 1) {
            if (!result.empty()) result += " + ";
            if (i == 0) {
                result += "1";
            }
            else {
                bool first = true;
                for (int j = 0; j < n; j++) {
                    if (i & (1 << (n - j - 1))) {
                        if (!first) result += "&";
                        result += string(1, truth_table.getVariables()[j]);
                        first = false;
                    }
                }
            }
        }
    }
    return result.empty() ? "0" : result;
}