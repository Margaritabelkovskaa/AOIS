#include "CalculationMethod.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

CalculationMethod::CalculationMethod(const TruthTable& tt) : truth_table(tt) {}

vector<int> CalculationMethod::getMintermValues(int idx) const {
    int n = truth_table.getVariables().size();
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = (idx >> (n - i - 1)) & 1;
    }
    return result;
}

bool CalculationMethod::tryGlue(const Term& a, const Term& b, Term& result) {
    return a.canGlueWith(b, result);
}

void CalculationMethod::addUnique(vector<Term>& list, const Term& term) {
    for (const auto& t : list) if (t == term) return;
    list.push_back(term);
}

void CalculationMethod::removeDuplicates(vector<Term>& terms) {
    sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
        return a.values < b.values;
        });
    terms.erase(unique(terms.begin(), terms.end()), terms.end());
}

vector<Term> CalculationMethod::glueOnce(vector<Term>& terms, bool& changed) {
    changed = false;
    vector<Term> next;
    vector<bool> used(terms.size(), false);

    for (size_t i = 0; i < terms.size(); i++) {
        for (size_t j = i + 1; j < terms.size(); j++) {
            Term glued(truth_table.getVariables().size());
            if (tryGlue(terms[i], terms[j], glued)) {
                addUnique(next, glued);
                used[i] = true;
                used[j] = true;
                changed = true;
            }
        }
    }

    for (size_t i = 0; i < terms.size(); i++) {
        if (!used[i]) next.push_back(terms[i]);
    }

    removeDuplicates(next);
    return next;
}

bool CalculationMethod::isEssential(const Term& imp, const vector<Term>& all, int minterm) {
    vector<int> mv = getMintermValues(minterm);
    if (!imp.covers(mv)) return false;
    for (const auto& other : all) {
        if (other.values == imp.values) continue;
        if (other.covers(mv)) return false;
    }
    return true;
}

void CalculationMethod::collectEssential(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered) {
    const auto& tm = truth_table.getTrueMinterms();
    for (size_t i = 0; i < implicants.size(); i++) {
        for (size_t j = 0; j < tm.size(); j++) {
            if (isEssential(implicants[i], implicants, tm[j])) {
                result.push_back(implicants[i]);
                for (size_t k = 0; k < tm.size(); k++) {
                    if (implicants[i].covers(getMintermValues(tm[k]))) covered[k] = true;
                }
                break;
            }
        }
    }
}

void CalculationMethod::collectRemaining(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered) {
    const auto& tm = truth_table.getTrueMinterms();
    for (size_t i = 0; i < implicants.size(); i++) {
        bool needed = false;
        for (size_t j = 0; j < tm.size(); j++) {
            if (!covered[j] && implicants[i].covers(getMintermValues(tm[j]))) {
                needed = true;
                break;
            }
        }
        if (needed) {
            result.push_back(implicants[i]);
            for (size_t j = 0; j < tm.size(); j++) {
                if (implicants[i].covers(getMintermValues(tm[j]))) covered[j] = true;
            }
        }
    }
}

vector<Term> CalculationMethod::removeRedundantImplicants(vector<Term>& implicants) {
    if (implicants.size() <= 1) return implicants;
    vector<Term> result;
    vector<bool> covered(truth_table.getTrueMinterms().size(), false);
    collectEssential(result, implicants, covered);
    collectRemaining(result, implicants, covered);
    return result;
}

void CalculationMethod::printInitialSDNF() {
    const auto& tm = truth_table.getTrueMinterms();
    cout << "Исходная СДНФ: ";
    for (size_t i = 0; i < tm.size(); i++) {
        if (i > 0) cout << " | ";
        cout << "(" << truth_table.getMintermString(tm[i]) << ")";
    }
    cout << "\n";
}

void CalculationMethod::printNumericSets() {
    const auto& tm = truth_table.getTrueMinterms();
    const auto& vars = truth_table.getVariables();
    cout << "Числовые наборы: ";
    for (int idx : tm) {
        cout << "(";
        for (size_t i = 0; i < vars.size(); i++) {
            cout << ((idx >> (vars.size() - i - 1)) & 1);
        }
        cout << ") ";
    }
    cout << "\n";
}

void CalculationMethod::printGluingResult(int stage, const vector<Term>& terms) {
    const auto& vars = truth_table.getVariables();
    cout << "\nРезультат склеивания " << stage << ":\n  ";
    for (size_t i = 0; i < terms.size(); i++) {
        if (i > 0) cout << " | ";
        cout << terms[i].toString(vars) << " " << terms[i].getNumericString();
    }
    cout << "\n";
}

vector<Term> CalculationMethod::getPrimeImplicants() {
    const auto& tm = truth_table.getTrueMinterms();
    int n = truth_table.getVariables().size();
    vector<Term> terms;
    for (int idx : tm) {
        Term t(n);
        for (int i = 0; i < n; i++) t.values[i] = (idx >> (n - i - 1)) & 1;
        terms.push_back(t);
    }
    if (terms.empty()) return terms;
    bool changed;
    do { terms = glueOnce(terms, changed); } while (changed && terms.size() > 1);
    return terms;
}

vector<Term> CalculationMethod::getPrimeImplicantsWithStages() {
    const auto& tm = truth_table.getTrueMinterms();
    const auto& vars = truth_table.getVariables();
    int n = vars.size();
    vector<Term> terms;
    for (int idx : tm) {
        Term t(n);
        for (int i = 0; i < n; i++) t.values[i] = (idx >> (n - i - 1)) & 1;
        terms.push_back(t);
    }
    if (terms.empty()) return terms;

    cout << "Исходные термы:\n  ";
    for (size_t i = 0; i < terms.size(); i++) {
        if (i > 0) cout << " | ";
        cout << terms[i].toString(vars) << " " << terms[i].getNumericString();
    }
    cout << "\n";

    int stage = 1;
    bool changed;
    while (terms.size() > 1) {
        vector<Term> next = glueOnce(terms, changed);
        if (!changed) break;
        cout << "\nСклеивание " << stage << ":\n";
        printGluingResult(stage, next);
        terms = next;
        stage++;
    }
    return terms;
}

void CalculationMethod::minimize() {
    cout << "\n=== Расчетный метод минимизации ===\n";
    const auto& tm = truth_table.getTrueMinterms();
    const auto& vars = truth_table.getVariables();
    if (tm.empty()) {
        cout << "Функция тождественно равна 0\n";
        return;
    }
    printInitialSDNF();
    printNumericSets();

    int n = vars.size();
    vector<Term> terms;
    for (int idx : tm) {
        Term t(n);
        for (int i = 0; i < n; i++) t.values[i] = (idx >> (n - i - 1)) & 1;
        terms.push_back(t);
    }

    cout << "\n=== Этап склеивания ===\n";
    int stage = 1;
    bool changed;
    while (terms.size() > 1) {
        vector<Term> next = glueOnce(terms, changed);
        if (!changed) break;
        cout << "\nСклеивание " << stage << ":\n";
        printGluingResult(stage, next);
        terms = next;
        stage++;
    }

    cout << "\n=== Удаление лишних импликант ===\n";
    vector<Term> result = removeRedundantImplicants(terms);
    string sdnf;
    for (size_t i = 0; i < result.size(); i++) {
        if (i > 0) sdnf += " | ";
        sdnf += result[i].toString(vars);
    }
    cout << "\nРезультат минимизации: " << sdnf << "\n";
}