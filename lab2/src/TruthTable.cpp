#include "TruthTable.h"
#include <iostream>
#include <cmath>

using namespace std;

TruthTable::TruthTable(const ExpressionParser& parser) {
    variables = parser.getVariables();
    int n = variables.size();
    int rows = pow(2, n);

    table.clear();
    true_minterms.clear();
    false_minterms.clear();

    for (int i = 0; i < rows; i++) {
        vector<int> inputs(n);
        for (int j = 0; j < n; j++) {
            inputs[j] = (i >> (n - j - 1)) & 1;
        }
        bool result = parser.compute(inputs);

        vector<int> row = inputs;
        row.push_back(result);
        table.push_back(row);

        if (result) {
            true_minterms.push_back(i);
        }
        else {
            false_minterms.push_back(i);
        }
    }
}

string TruthTable::getMintermString(int index) const {
    int n = variables.size();
    string result;
    for (int i = 0; i < n; i++) {
        if (i > 0) result += " & ";
        bool val = (index >> (n - i - 1)) & 1;
        if (val == 1) {
            result += string(1, variables[i]);
        }
        else {
            result += "!" + string(1, variables[i]);
        }
    }
    return result;
}

string TruthTable::getMaxtermString(int index) const {
    int n = variables.size();
    string result = "(";
    for (int i = 0; i < n; i++) {
        if (i > 0) result += " | ";
        bool val = (index >> (n - i - 1)) & 1;
        if (val == 1) {
            result += "!" + string(1, variables[i]);
        }
        else {
            result += string(1, variables[i]);
        }
    }
    result += ")";
    return result;
}

void TruthTable::print() const {
    cout << "\n=== Таблица истинности ===\n";
    for (char v : variables) {
        cout << v << " ";
    }
    cout << "| f\n";
    cout << string(variables.size() * 2 + 2, '-') << "\n";

    for (const auto& row : table) {
        for (size_t i = 0; i < row.size() - 1; i++) {
            cout << row[i] << " ";
        }
        cout << "| " << row.back() << "\n";
    }
}