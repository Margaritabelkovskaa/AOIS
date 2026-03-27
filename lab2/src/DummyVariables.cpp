#include "DummyVariables.h"
#include <iostream>
#include <cmath>

using namespace std;

DummyVariables::DummyVariables(const TruthTable& tt) : truth_table(tt) {
    computeDummyVariables();
}

void DummyVariables::computeDummyVariables() {
    int n = truth_table.getVariables().size();
    if (n == 0) {
        isDummy.clear();
        return;
    }
    
    isDummy.assign(n, true);
    
    for (int var = 0; var < n; var++) {
        for (int i = 0; i < (1 << n); i++) {
            int j = i ^ (1 << (n - var - 1));
            if (truth_table.getTable()[i].back() != truth_table.getTable()[j].back()) {
                isDummy[var] = false;
                break;
            }
        }
    }
}

bool DummyVariables::isVariableDummy(int idx) const {
    if (idx < 0 || idx >= (int)isDummy.size()) {
        return false;
    }
    return isDummy[idx];
}

vector<char> DummyVariables::getDummyVariables() const {
    vector<char> result;
    const auto& vars = truth_table.getVariables();
    for (size_t i = 0; i < isDummy.size(); i++) {
        if (isDummy[i] && i < vars.size()) {
            result.push_back(vars[i]);
        }
    }
    return result;
}

vector<char> DummyVariables::getEssentialVariables() const {
    vector<char> result;
    const auto& vars = truth_table.getVariables();
    for (size_t i = 0; i < isDummy.size(); i++) {
        if (!isDummy[i] && i < vars.size()) {
            result.push_back(vars[i]);
        }
    }
    return result;
}

bool DummyVariables::hasDummyVariables() const {
    for (bool dummy : isDummy) {
        if (dummy) return true;
    }
    return false;
}

void DummyVariables::print() const {
    cout << "\n=== Фиктивные переменные ===\n";
    
    int n = truth_table.getVariables().size();
    if (n == 0) {
        cout << "Нет переменных\n";
        return;
    }
    
    bool found = false;
    const auto& vars = truth_table.getVariables();
    for (size_t i = 0; i < isDummy.size(); i++) {
        if (isDummy[i]) {
            cout << vars[i] << " - фиктивная переменная\n";
            found = true;
        }
    }
    
    if (!found) {
        cout << "Фиктивных переменных нет\n";
    }
}

int DummyVariables::getDummyCount() const {
    int count = 0;
    for (bool dummy : isDummy) {
        if (dummy) count++;
    }
    return count;
}

int DummyVariables::getEssentialCount() const {
    return isDummy.size() - getDummyCount();
}
