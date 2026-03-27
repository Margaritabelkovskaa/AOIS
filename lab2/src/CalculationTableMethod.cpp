#include "CalculationTableMethod.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

CalculationTableMethod::CalculationTableMethod(const TruthTable& tt) : truth_table(tt), calc(tt) {}

vector<int> CalculationTableMethod::getMintermValues(int idx) const {
    int n = truth_table.getVariables().size();
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = (idx >> (n - i - 1)) & 1;
    }
    return result;
}

int CalculationTableMethod::findBestImplicant(const vector<vector<bool>>& cov, const vector<bool>& covered) {
    int best = -1, bestCnt = 0;
    for (size_t i = 0; i < cov.size(); i++) {
        int cnt = 0;
        for (size_t j = 0; j < covered.size(); j++) {
            if (cov[i][j] && !covered[j]) cnt++;
        }
        if (cnt > bestCnt) { bestCnt = cnt; best = i; }
    }
    return best;
}

vector<int> CalculationTableMethod::selectMinCoverage(vector<vector<bool>>& coverage) {
    const auto& tm = truth_table.getTrueMinterms();
    vector<int> selected;
    vector<bool> covered(tm.size(), false);
    while (true) {
        int best = findBestImplicant(coverage, covered);
        if (best == -1) break;
        selected.push_back(best);
        for (size_t j = 0; j < tm.size(); j++) {
            if (coverage[best][j]) covered[j] = true;
        }
    }
    return selected;
}

void CalculationTableMethod::printCoverageTable(const vector<Term>& implicants, const vector<int>& minterms,
    const vector<vector<bool>>& cov) {
    cout << "     ";
    for (int idx : minterms) cout << "m" << idx << " ";
    cout << "\n";
    cout << "     " << string(minterms.size() * 3, '-') << "\n";
    for (size_t i = 0; i < implicants.size(); i++) {
        cout << "  " << char('A' + i) << ": ";
        for (size_t j = 0; j < minterms.size(); j++) {
            cout << (cov[i][j] ? " X " : "   ");
        }
        cout << "\n";
    }
}

void CalculationTableMethod::minimize() {
    cout << "\n=== Расчетно-табличный метод минимизации ===\n";
    const auto& tm = truth_table.getTrueMinterms();
    const auto& vars = truth_table.getVariables();
    if (tm.empty()) {
        cout << "Функция тождественно равна 0\n";
        return;
    }

    cout << "=== Этап склеивания ===\n";
    vector<Term> prime = calc.getPrimeImplicantsWithStages();

    cout << "\n=== Простые импликанты ===\n";
    for (size_t i = 0; i < prime.size(); i++) {
        cout << "  " << char('A' + i) << ": " << prime[i].toString(vars);
        cout << " " << prime[i].getNumericString() << "\n";
    }

    cout << "\n=== Таблица покрытия ===\n";
    vector<vector<bool>> cov(prime.size(), vector<bool>(tm.size(), false));
    for (size_t i = 0; i < prime.size(); i++) {
        for (size_t j = 0; j < tm.size(); j++) {
            if (prime[i].covers(getMintermValues(tm[j]))) cov[i][j] = true;
        }
    }
    printCoverageTable(prime, tm, cov);

    cout << "\n=== Выбор минимального покрытия ===\n";
    vector<int> selected = selectMinCoverage(cov);
    string result;
    for (int i : selected) {
        if (!result.empty()) result += " | ";
        result += prime[i].toString(vars);
    }
    cout << "\nМинимизированная функция: " << result << "\n";
}