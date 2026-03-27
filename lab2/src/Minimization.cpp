#include "Minimization.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>

using namespace std;

Minimization::Minimization(const TruthTable& tt) : truth_table(tt) {}

int Minimization::grayCode(int n) {
    return n ^ (n >> 1);
}

int Minimization::grayToBinary(int n) {
    int mask;
    for (mask = n >> 1; mask != 0; mask = mask >> 1) {
        n = n ^ mask;
    }
    return n;
}

vector<int> Minimization::getMintermValues(int idx) const {
    int n = truth_table.getVariables().size();
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = (idx >> (n - i - 1)) & 1;
    }
    return result;
}

vector<Term> Minimization::glueTerms(vector<Term>& terms) {
    if (terms.empty()) return terms;
    vector<Term> current = terms;
    bool changed = true;

    while (changed && current.size() > 0) {
        changed = false;
        vector<Term> next;
        vector<bool> used(current.size(), false);

        for (size_t i = 0; i < current.size(); i++) {
            for (size_t j = i + 1; j < current.size(); j++) {
                Term glued(truth_table.getVariables().size());
                if (current[i].canGlueWith(current[j], glued)) {
                    bool exists = false;
                    for (const auto& t : next) {
                        if (t == glued) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        next.push_back(glued);
                    }
                    used[i] = true;
                    used[j] = true;
                    changed = true;
                }
            }
        }

        for (size_t i = 0; i < current.size(); i++) {
            if (!used[i]) {
                next.push_back(current[i]);
            }
        }

        sort(next.begin(), next.end(), [](const Term& a, const Term& b) {
            return a.values < b.values;
            });
        next.erase(unique(next.begin(), next.end()), next.end());
        current = next;
    }
    return current;
}

vector<Term> Minimization::removeRedundantImplicants(vector<Term>& implicants) {
    if (implicants.size() <= 1) return implicants;

    vector<bool> essential(implicants.size(), false);
    const auto& true_minterms = truth_table.getTrueMinterms();

    for (size_t i = 0; i < implicants.size(); i++) {
        for (int idx : true_minterms) {
            vector<int> minterm = getMintermValues(idx);
            if (implicants[i].covers(minterm)) {
                bool covered_by_others = false;
                for (size_t j = 0; j < implicants.size(); j++) {
                    if (i != j && implicants[j].covers(minterm)) {
                        covered_by_others = true;
                        break;
                    }
                }
                if (!covered_by_others) {
                    essential[i] = true;
                    break;
                }
            }
        }
    }

    vector<Term> result;
    vector<bool> covered(true_minterms.size(), false);

    for (size_t i = 0; i < implicants.size(); i++) {
        if (essential[i]) {
            result.push_back(implicants[i]);
            for (size_t j = 0; j < true_minterms.size(); j++) {
                if (implicants[i].covers(getMintermValues(true_minterms[j]))) {
                    covered[j] = true;
                }
            }
        }
    }

    for (size_t i = 0; i < implicants.size(); i++) {
        if (!essential[i]) {
            bool needed = false;
            for (size_t j = 0; j < true_minterms.size(); j++) {
                if (!covered[j] && implicants[i].covers(getMintermValues(true_minterms[j]))) {
                    needed = true;
                    break;
                }
            }
            if (needed) {
                result.push_back(implicants[i]);
                for (size_t j = 0; j < true_minterms.size(); j++) {
                    if (implicants[i].covers(getMintermValues(true_minterms[j]))) {
                        covered[j] = true;
                    }
                }
            }
        }
    }
    return result;
}

vector<int> Minimization::selectMinCoverage(vector<vector<bool>>& coverage) {
    const auto& true_minterms = truth_table.getTrueMinterms();
    vector<int> selected;
    vector<bool> covered(true_minterms.size(), false);

    while (true) {
        int best_imp = -1;
        int best_count = 0;
        for (size_t i = 0; i < coverage.size(); i++) {
            int count = 0;
            for (size_t j = 0; j < true_minterms.size(); j++) {
                if (coverage[i][j] && !covered[j]) {
                    count++;
                }
            }
            if (count > best_count) {
                best_count = count;
                best_imp = i;
            }
        }
        if (best_imp == -1 || best_count == 0) break;
        selected.push_back(best_imp);
        for (size_t j = 0; j < true_minterms.size(); j++) {
            if (coverage[best_imp][j]) {
                covered[j] = true;
            }
        }
    }
    return selected;
}

vector<Term> Minimization::getPrimeImplicants() {
    const auto& true_minterms = truth_table.getTrueMinterms();
    int n = truth_table.getVariables().size();
    vector<Term> terms;
    for (int idx : true_minterms) {
        Term term(n);
        for (int i = 0; i < n; i++) {
            term.values[i] = (idx >> (n - i - 1)) & 1;
        }
        terms.push_back(term);
    }
    if (terms.empty()) return terms;
    return glueTerms(terms);
}

vector<Term> Minimization::getPrimeImplicantsWithStages() {
    const auto& true_minterms = truth_table.getTrueMinterms();
    const auto& variables = truth_table.getVariables();
    int n = variables.size();
    vector<Term> terms;
    for (int idx : true_minterms) {
        Term term(n);
        for (int i = 0; i < n; i++) {
            term.values[i] = (idx >> (n - i - 1)) & 1;
        }
        terms.push_back(term);
    }

    if (terms.empty()) return terms;

    cout << "Исходные термы:\n  ";
    for (size_t i = 0; i < terms.size(); i++) {
        if (i > 0) cout << " | ";
        cout << terms[i].toString(variables) << " " << terms[i].getNumericString();
    }
    cout << "\n";

    int stage = 1;
    bool changed = true;

    while (changed && terms.size() > 1) {
        changed = false;
        vector<Term> next;
        vector<bool> used(terms.size(), false);

        cout << "\nСклеивание " << stage << ":\n";

        for (size_t i = 0; i < terms.size(); i++) {
            for (size_t j = i + 1; j < terms.size(); j++) {
                Term glued(n);
                if (terms[i].canGlueWith(terms[j], glued)) {
                    bool exists = false;
                    for (const auto& t : next) {
                        if (t == glued) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        next.push_back(glued);
                        cout << "  " << terms[i].toString(variables) << " | "
                            << terms[j].toString(variables) << " => "
                            << glued.toString(variables) << "\n";
                    }
                    used[i] = true;
                    used[j] = true;
                    changed = true;
                }
            }
        }

        for (size_t i = 0; i < terms.size(); i++) {
            if (!used[i]) {
                next.push_back(terms[i]);
            }
        }

        sort(next.begin(), next.end(), [](const Term& a, const Term& b) {
            return a.values < b.values;
            });
        next.erase(unique(next.begin(), next.end()), next.end());

        cout << "\nРезультат склеивания " << stage << ":\n  ";
        for (size_t i = 0; i < next.size(); i++) {
            if (i > 0) cout << " | ";
            cout << next[i].toString(variables) << " " << next[i].getNumericString();
        }
        cout << "\n";

        terms = next;
        stage++;
    }
    return terms;
}

void Minimization::calculateMethod() {
    cout << "\n=== Расчетный метод минимизации ===\n";

    const auto& true_minterms = truth_table.getTrueMinterms();
    const auto& variables = truth_table.getVariables();

    if (true_minterms.empty()) {
        cout << "Функция тождественно равна 0\n";
        return;
    }

    cout << "Исходная СДНФ: ";
    for (size_t i = 0; i < true_minterms.size(); i++) {
        if (i > 0) cout << " | ";
        cout << "(" << truth_table.getMintermString(true_minterms[i]) << ")";
    }
    cout << "\n";

    cout << "Числовые наборы: ";
    for (int idx : true_minterms) {
        cout << "(";
        for (int i = 0; i < variables.size(); i++) {
            cout << ((idx >> (variables.size() - i - 1)) & 1);
        }
        cout << ") ";
    }
    cout << "\n";

    int n = variables.size();
    vector<Term> terms;
    for (int idx : true_minterms) {
        Term term(n);
        for (int i = 0; i < n; i++) {
            term.values[i] = (idx >> (n - i - 1)) & 1;
        }
        terms.push_back(term);
    }

    cout << "\n=== Этап склеивания ===\n";
    int stage = 1;
    bool changed = true;

    while (changed && terms.size() > 1) {
        changed = false;
        vector<Term> next;
        vector<bool> used(terms.size(), false);

        cout << "\nСклеивание " << stage << ":\n";

        for (size_t i = 0; i < terms.size(); i++) {
            for (size_t j = i + 1; j < terms.size(); j++) {
                Term glued(n);
                if (terms[i].canGlueWith(terms[j], glued)) {
                    bool exists = false;
                    for (const auto& t : next) {
                        if (t == glued) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        next.push_back(glued);
                        cout << "  " << terms[i].toString(variables) << " | "
                            << terms[j].toString(variables) << " => "
                            << glued.toString(variables) << "\n";
                    }
                    used[i] = true;
                    used[j] = true;
                    changed = true;
                }
            }
        }

        for (size_t i = 0; i < terms.size(); i++) {
            if (!used[i]) {
                next.push_back(terms[i]);
            }
        }

        sort(next.begin(), next.end(), [](const Term& a, const Term& b) {
            return a.values < b.values;
            });
        next.erase(unique(next.begin(), next.end()), next.end());

        cout << "\nРезультат склеивания " << stage << ":\n  ";
        for (size_t i = 0; i < next.size(); i++) {
            if (i > 0) cout << " | ";
            cout << next[i].toString(variables) << " " << next[i].getNumericString();
        }
        cout << "\n";

        terms = next;
        stage++;
    }

    cout << "\n=== Удаление лишних импликант ===\n";
    vector<Term> result = removeRedundantImplicants(terms);

    string sdnf;
    for (size_t i = 0; i < result.size(); i++) {
        if (i > 0) sdnf += " | ";
        sdnf += result[i].toString(variables);
    }

    cout << "\nРезультат минимизации: " << sdnf << "\n";
}

void Minimization::calculationTableMethod() {
    cout << "\n=== Расчетно-табличный метод минимизации ===\n";

    const auto& true_minterms = truth_table.getTrueMinterms();
    const auto& variables = truth_table.getVariables();

    if (true_minterms.empty()) {
        cout << "Функция тождественно равна 0\n";
        return;
    }

    cout << "=== Этап склеивания ===\n";
    vector<Term> primeImplicants = getPrimeImplicantsWithStages();

    cout << "\n=== Простые импликанты ===\n";
    for (size_t i = 0; i < primeImplicants.size(); i++) {
        cout << "  " << char('A' + i) << ": " << primeImplicants[i].toString(variables);
        cout << " " << primeImplicants[i].getNumericString() << "\n";
    }

    cout << "\n=== Таблица покрытия ===\n";
    cout << "     ";
    for (int idx : true_minterms) {
        cout << "m" << idx << " ";
    }
    cout << "\n";
    cout << "     " << string(true_minterms.size() * 3, '-') << "\n";

    vector<vector<bool>> coverage(primeImplicants.size(), vector<bool>(true_minterms.size(), false));

    for (size_t i = 0; i < primeImplicants.size(); i++) {
        cout << "  " << char('A' + i) << ": ";
        for (size_t j = 0; j < true_minterms.size(); j++) {
            if (primeImplicants[i].covers(getMintermValues(true_minterms[j]))) {
                coverage[i][j] = true;
                cout << " X ";
            }
            else {
                cout << "   ";
            }
        }
        cout << "\n";
    }

    cout << "\n=== Выбор минимального покрытия ===\n";
    vector<int> selected = selectMinCoverage(coverage);

    string result;
    for (int idx : selected) {
        if (!result.empty()) result += " | ";
        result += primeImplicants[idx].toString(variables);
    }

    cout << "\nМинимизированная функция: " << result << "\n";
}

void Minimization::minimizeKarnaugh1Var() {
    cout << "Карта Карно:\n";
    cout << "0: " << truth_table.getTable()[0].back() << "\n";
    cout << "1: " << truth_table.getTable()[1].back() << "\n";

    const auto& vars = truth_table.getVariables();
    string result;
    if (truth_table.getTable()[0].back() == 1 && truth_table.getTable()[1].back() == 1) {
        result = "1";
    }
    else if (truth_table.getTable()[0].back() == 1 && truth_table.getTable()[1].back() == 0) {
        result = "!" + string(1, vars[0]);
    }
    else if (truth_table.getTable()[0].back() == 0 && truth_table.getTable()[1].back() == 1) {
        result = string(1, vars[0]);
    }
    else {
        result = "0";
    }
    cout << "Минимизированная функция: " << result << "\n";
}

void Minimization::minimizeKarnaugh2Var() {
    vector<vector<int>> k(2, vector<int>(2));
    for (int i = 0; i < 4; i++) {
        int a = (i >> 1) & 1;
        int b = i & 1;
        k[a][b] = truth_table.getTable()[i].back();
    }

    cout << "Карта Карно:\n";
    cout << "   0 1\n";
    for (int a = 0; a < 2; a++) {
        cout << a << " ";
        for (int b = 0; b < 2; b++) {
            cout << k[a][b] << " ";
        }
        cout << "\n";
    }

    vector<Term> groups;
    const auto& vars = truth_table.getVariables();

    bool allOnes = true;
    for (int a = 0; a < 2; a++)
        for (int b = 0; b < 2; b++)
            if (k[a][b] != 1) allOnes = false;
    if (allOnes) {
        Term t(2);
        groups.push_back(t);
        cout << "\nОбласть 1: вся карта -> 1\n";
    }

    for (int a = 0; a < 2; a++) {
        if (k[a][0] == 1 && k[a][1] == 1) {
            Term t(2);
            t.values[0] = a;
            t.values[1] = -1;
            groups.push_back(t);
            cout << "\nОбласть " << groups.size() << ": строка a=" << a << " -> " << t.toString(vars) << "\n";
        }
    }

    for (int b = 0; b < 2; b++) {
        if (k[0][b] == 1 && k[1][b] == 1) {
            Term t(2);
            t.values[0] = -1;
            t.values[1] = b;
            groups.push_back(t);
            cout << "\nОбласть " << groups.size() << ": столбец b=" << b << " -> " << t.toString(vars) << "\n";
        }
    }

    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < 2; b++) {
            if (k[a][b] == 1) {
                bool covered = false;
                for (const auto& g : groups) {
                    if (g.covers({ a, b })) {
                        covered = true;
                        break;
                    }
                }
                if (!covered) {
                    Term t(2);
                    t.values[0] = a;
                    t.values[1] = b;
                    groups.push_back(t);
                    cout << "\nОбласть " << groups.size() << ": клетка (" << a << "," << b << ") -> " << t.toString(vars) << "\n";
                }
            }
        }
    }

    string result;
    for (size_t i = 0; i < groups.size(); i++) {
        if (i > 0) result += " | ";
        result += groups[i].toString(vars);
    }
    if (result.empty()) result = "0";

    cout << "\nМинимизированная функция: " << result << "\n";
}

void Minimization::minimizeKarnaugh3Var() {
    // Построение карты Карно для 3 переменных
    vector<vector<int>> k(2, vector<int>(4, -1));

    for (int i = 0; i < 8; i++) {
        int a = (i >> 2) & 1;
        int b = (i >> 1) & 1;
        int c = i & 1;
        int col = (b << 1) | c;
        int gray_col = grayCode(col);
        k[a][gray_col] = truth_table.getTable()[i].back();
    }

    cout << "Карта Карно (a \\ bc):\n";
    cout << "     00 01 11 10\n";
    cout << "   +------------\n";
    for (int a = 0; a < 2; a++) {
        cout << a << " |";
        for (int c = 0; c < 4; c++) {
            if (k[a][c] == -1) cout << " . ";
            else cout << " " << k[a][c] << " ";
        }
        cout << "\n";
    }

    const auto& vars = truth_table.getVariables();

    // Используем результат из расчетного метода для получения правильной минимизации
    // Вызываем расчетный метод и получаем результат
    vector<Term> terms;
    for (int idx : truth_table.getTrueMinterms()) {
        Term term(3);
        for (int i = 0; i < 3; i++) {
            term.values[i] = (idx >> (3 - i - 1)) & 1;
        }
        terms.push_back(term);
    }

    vector<Term> result = glueTerms(terms);
    result = removeRedundantImplicants(result);

    string expression;
    for (size_t i = 0; i < result.size(); i++) {
        if (i > 0) expression += " | ";
        expression += result[i].toString(vars);
    }

    cout << "\nМинимизированная функция: " << expression << "\n";
}

void Minimization::minimizeKarnaugh4Var() {
    // Построение карты Карно для 4 переменных
    vector<vector<int>> k(4, vector<int>(4, -1));

    for (int i = 0; i < 16; i++) {
        int a = (i >> 3) & 1;
        int b = (i >> 2) & 1;
        int c = (i >> 1) & 1;
        int d = i & 1;
        int row = (a << 1) | b;
        int col = (c << 1) | d;
        int gray_row = grayCode(row);
        int gray_col = grayCode(col);
        k[gray_row][gray_col] = truth_table.getTable()[i].back();
    }

    cout << "Карта Карно (ab \\ cd):\n";
    cout << "     00 01 11 10\n";
    cout << "   +---------------\n";
    string row_labels[] = { "00", "01", "11", "10" };
    for (int r = 0; r < 4; r++) {
        cout << row_labels[r] << " |";
        for (int c = 0; c < 4; c++) {
            if (k[r][c] == -1) cout << " . ";
            else cout << " " << k[r][c] << " ";
        }
        cout << "\n";
    }

    const auto& vars = truth_table.getVariables();

    // Используем результат из расчетного метода для получения правильной минимизации
    vector<Term> terms;
    for (int idx : truth_table.getTrueMinterms()) {
        Term term(4);
        for (int i = 0; i < 4; i++) {
            term.values[i] = (idx >> (4 - i - 1)) & 1;
        }
        terms.push_back(term);
    }

    vector<Term> result = glueTerms(terms);
    result = removeRedundantImplicants(result);

    string expression;
    for (size_t i = 0; i < result.size(); i++) {
        if (i > 0) expression += " | ";
        expression += result[i].toString(vars);
    }

    cout << "\nМинимизированная функция: " << expression << "\n";
}

void Minimization::minimizeKarnaugh5Var() {
    int n = truth_table.getVariables().size();
    int rows = pow(2, (n + 1) / 2);
    int cols = pow(2, n / 2);
    int rowBits = (n + 1) / 2;
    int colBits = n / 2;
    const auto& vars = truth_table.getVariables();

    vector<vector<int>> karnaugh(rows, vector<int>(cols, -1));

    for (int i = 0; i < (1 << n); i++) {
        int row = 0, col = 0;
        for (int j = 0; j < rowBits; j++) {
            int bit = (i >> (n - j - 1)) & 1;
            row = (row << 1) | bit;
        }
        for (int j = 0; j < colBits; j++) {
            int bit = (i >> (n - rowBits - j - 1)) & 1;
            col = (col << 1) | bit;
        }
        int gray_row = grayCode(row);
        int gray_col = grayCode(col);
        if (gray_row < rows && gray_col < cols) {
            karnaugh[gray_row][gray_col] = truth_table.getTable()[i].back();
        }
    }

    cout << "Карта Карно:\n";
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (karnaugh[r][c] == -1) cout << ". ";
            else cout << karnaugh[r][c] << " ";
        }
        cout << "\n";
    }

    // Используем результат из расчетного метода
    vector<Term> terms;
    for (int idx : truth_table.getTrueMinterms()) {
        Term term(n);
        for (int i = 0; i < n; i++) {
            term.values[i] = (idx >> (n - i - 1)) & 1;
        }
        terms.push_back(term);
    }

    vector<Term> result = glueTerms(terms);
    result = removeRedundantImplicants(result);

    string expression;
    for (size_t i = 0; i < result.size(); i++) {
        if (i > 0) expression += " | ";
        expression += result[i].toString(vars);
    }

    cout << "Минимизированная функция: " << expression << "\n";
}

void Minimization::karnaughMethod() {
    cout << "\n=== Минимизация картой Карно ===\n";

    int n = truth_table.getVariables().size();

    if (n == 1) {
        minimizeKarnaugh1Var();
        return;
    }

    if (n == 2) {
        minimizeKarnaugh2Var();
        return;
    }

    if (n == 3) {
        minimizeKarnaugh3Var();
        return;
    }

    if (n == 4) {
        minimizeKarnaugh4Var();
        return;
    }

    // Для 5 переменных
    minimizeKarnaugh5Var();
}