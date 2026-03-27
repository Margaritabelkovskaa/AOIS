
#include "KarnaughMethod.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

KarnaughMethod::KarnaughMethod(const TruthTable& tt) : truth_table(tt), calc(tt) {}

int KarnaughMethod::grayCode(int n) {
    return n ^ (n >> 1);
}

int KarnaughMethod::grayToBinary(int n) {
    int mask;
    for (mask = n >> 1; mask != 0; mask = mask >> 1) {
        n = n ^ mask;
    }
    return n;
}

vector<int> KarnaughMethod::getMintermValues(int idx) const {
    int n = truth_table.getVariables().size();
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = (idx >> (n - i - 1)) & 1;
    }
    return result;
}

string KarnaughMethod::groupsToString(const vector<Term>& groups) {
    string result;
    for (size_t i = 0; i < groups.size(); i++) {
        if (i > 0) result += " | ";
        result += groups[i].toString(truth_table.getVariables());
    }
    return result.empty() ? "0" : result;
}

void KarnaughMethod::splitVariables(int n, int& layerBits, int& rowBits, int& colBits) {
    if (n <= 2) {
        layerBits = 0;
        rowBits = n;
        colBits = 0;
        if (n == 2) { rowBits = 1; colBits = 1; }
        if (n == 1) { rowBits = 1; colBits = 0; }
    }
    else if (n == 3) {
        layerBits = 0;
        rowBits = 1;
        colBits = 2;
    }
    else if (n == 4) {
        layerBits = 0;
        rowBits = 2;
        colBits = 2;
    }
    else { // n == 5
        layerBits = 1;
        rowBits = 2;
        colBits = 2;
    }
}

vector<vector<int>> KarnaughMethod::buildKarnaughMap() {
    int n = truth_table.getVariables().size();
    int layerBits, rowBits, colBits;
    splitVariables(n, layerBits, rowBits, colBits);

    int layers = (layerBits == 0) ? 1 : (1 << layerBits);
    int rows = (rowBits == 0) ? 1 : (1 << rowBits);
    int cols = (colBits == 0) ? 1 : (1 << colBits);

    vector<vector<int>> map(layers * rows, vector<int>(cols, -1));

    for (int i = 0; i < (1 << n); i++) {
        int layer = 0, row = 0, col = 0;
        int bitPos = 0;

        for (int j = 0; j < layerBits; j++) {
            layer = (layer << 1) | ((i >> (n - bitPos - 1)) & 1);
            bitPos++;
        }
        for (int j = 0; j < rowBits; j++) {
            row = (row << 1) | ((i >> (n - bitPos - 1)) & 1);
            bitPos++;
        }
        for (int j = 0; j < colBits; j++) {
            col = (col << 1) | ((i >> (n - bitPos - 1)) & 1);
            bitPos++;
        }

        int gray_layer = (layerBits == 0) ? 0 : grayCode(layer);
        int gray_row = (rowBits == 0) ? 0 : grayCode(row);
        int gray_col = (colBits == 0) ? 0 : grayCode(col);

        map[gray_layer * rows + gray_row][gray_col] = truth_table.getTable()[i].back();
    }
    return map;
}

void KarnaughMethod::printKarnaughMap(const vector<vector<int>>& map, int layerBits, int rowBits, int colBits) {
    int n = truth_table.getVariables().size();
    const auto& vars = truth_table.getVariables();

    if (n == 1) {
        cout << "Карта Карно:\n";
        cout << "0: " << map[0][0] << "\n";
        cout << "1: " << map[1][0] << "\n";
        return;
    }

    if (n == 2) {
        cout << "Карта Карно:\n";
        cout << "   0 1\n";
        for (int r = 0; r < 2; r++) {
            cout << r << " ";
            for (int c = 0; c < 2; c++) {
                cout << map[r][c] << " ";
            }
            cout << "\n";
        }
        return;
    }

    if (n == 3) {
        cout << "Карта Карно (a \\ bc):\n";
        cout << "     00 01 11 10\n";
        cout << "   +------------\n";
        for (int r = 0; r < 2; r++) {
            cout << r << " |";
            for (int c = 0; c < 4; c++) {
                if (map[r][c] == -1) cout << " . ";
                else cout << " " << map[r][c] << " ";
            }
            cout << "\n";
        }
        return;
    }

    if (n == 4) {
        cout << "Карта Карно (ab \\ cd):\n";
        cout << "     00 01 11 10\n";
        cout << "   +---------------\n";
        string rowLabels[] = { "00", "01", "11", "10" };
        for (int r = 0; r < 4; r++) {
            cout << rowLabels[r] << " |";
            for (int c = 0; c < 4; c++) {
                if (map[r][c] == -1) cout << " . ";
                else cout << " " << map[r][c] << " ";
            }
            cout << "\n";
        }
        return;
    }

    if (n == 5) {
        int rows = map.size() / 2;
        string layerLabels[] = { "a=0", "a=1" };
        string rowLabels[] = { "00", "01", "11", "10" };

        cout << "Карта Карно (слои по a, строки bc, столбцы de):\n";
        for (int l = 0; l < 2; l++) {
            cout << "\n[" << layerLabels[l] << "]\n";
            cout << "     00 01 11 10\n";
            cout << "   +---------------\n";
            for (int r = 0; r < 4; r++) {
                cout << rowLabels[r] << " |";
                for (int c = 0; c < 4; c++) {
                    int val = map[l * 4 + r][c];
                    if (val == -1) cout << " . ";
                    else cout << " " << val << " ";
                }
                cout << "\n";
            }
        }
        return;
    }
}

vector<Term> KarnaughMethod::createTermsFromMinterms() {
    const auto& tm = truth_table.getTrueMinterms();
    int n = truth_table.getVariables().size();
    vector<Term> terms;
    for (int idx : tm) {
        Term t(n);
        for (int i = 0; i < n; i++) {
            t.values[i] = (idx >> (n - i - 1)) & 1;
        }
        terms.push_back(t);
    }
    return terms;
}

vector<Term> KarnaughMethod::minimizeByCalculation() {
    vector<Term> terms = createTermsFromMinterms();
    if (terms.empty()) return terms;

    bool changed;
    while (terms.size() > 1) {
        vector<Term> next = calc.glueOnce(terms, changed);
        if (!changed) break;
        terms = next;
    }
    return calc.removeRedundantImplicants(terms);
}

void KarnaughMethod::minimize() {
    cout << "\n=== Минимизация картой Карно ===\n";

    int n = truth_table.getVariables().size();
    int layerBits, rowBits, colBits;
    splitVariables(n, layerBits, rowBits, colBits);

    vector<vector<int>> map = buildKarnaughMap();
    printKarnaughMap(map, layerBits, rowBits, colBits);

    vector<Term> result = minimizeByCalculation();

    cout << "\nМинимизированная функция: " << groupsToString(result) << "\n";
}