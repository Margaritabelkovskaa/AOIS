#include "SKNFMinimization.h"

using namespace std;

// ====================  ŒÕ—“–” “Œ– ====================
SKNFMinimization::SKNFMinimization(const TruthTable& tt) : truth_table(tt) {}

// ==================== Œ¡Ÿ»≈ Ã≈“Œƒ€ ====================
vector<int> SKNFMinimization::getMaxtermValues(int idx) const {
    int n = truth_table.getVariables().size();
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        int val = (idx >> (n - i - 1)) & 1;
        result[i] = val;
    }
    return result;
}

int SKNFMinimization::grayCode(int n) {
    return n ^ (n >> 1);
}

int SKNFMinimization::grayToBinary(int n) {
    int mask;
    for (mask = n >> 1; mask != 0; mask = mask >> 1) {
        n = n ^ mask;
    }
    return n;
}

void SKNFMinimization::splitVariables(int n, int& layerBits, int& rowBits, int& colBits) {
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
    else {
        layerBits = 1;
        rowBits = 2;
        colBits = 2;
    }
}

string SKNFMinimization::maxtermToString(const vector<int>& values) const {
    const auto& vars = truth_table.getVariables();
    string result = "(";
    bool first = true;
    for (size_t i = 0; i < values.size(); i++) {
        if (values[i] != -1) {
            if (!first) result += " | ";
            if (values[i] == 0) result += vars[i];
            else result += "!" + string(1, vars[i]);
            first = false;
        }
    }
    result += ")";
    return result;
}

// ==================== Ã≈“Œƒ  ¬¿…Õ¿-Ã¿ - À¿— » (ÔÓ ÌÛÎˇÏ) ====================
bool SKNFMinimization::tryGlueMaxterm(const Term& a, const Term& b, Term& result) {
    int diff_count = 0;
    int diff_pos = -1;

    for (int i = 0; i < a.vars_count; i++) {
        if (a.values[i] != b.values[i]) {
            diff_count++;
            diff_pos = i;
            if (diff_count > 1) return false;
        }
    }

    if (diff_count == 1) {
        result = a;
        result.values[diff_pos] = -1;
        return true;
    }
    return false;
}

void SKNFMinimization::addUnique(vector<Term>& list, const Term& term) {
    for (const auto& t : list) if (t == term) return;
    list.push_back(term);
}

void SKNFMinimization::removeDuplicates(vector<Term>& terms) {
    sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
        return a.values < b.values;
        });
    terms.erase(unique(terms.begin(), terms.end()), terms.end());
}

vector<Term> SKNFMinimization::glueOnce(vector<Term>& terms, bool& changed) {
    changed = false;
    vector<Term> next;
    vector<bool> used(terms.size(), false);

    for (size_t i = 0; i < terms.size(); i++) {
        for (size_t j = i + 1; j < terms.size(); j++) {
            Term glued(truth_table.getVariables().size());
            if (tryGlueMaxterm(terms[i], terms[j], glued)) {
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

bool SKNFMinimization::isEssentialMaxterm(const Term& imp, const vector<Term>& all, int maxterm) {
    vector<int> mv = getMaxtermValues(maxterm);
    if (!imp.covers(mv)) return false;
    for (const auto& other : all) {
        if (other.values == imp.values) continue;
        if (other.covers(mv)) return false;
    }
    return true;
}

void SKNFMinimization::collectEssential(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered) {
    const auto& fm = truth_table.getFalseMinterms();
    for (size_t i = 0; i < implicants.size(); i++) {
        for (size_t j = 0; j < fm.size(); j++) {
            if (isEssentialMaxterm(implicants[i], implicants, fm[j])) {
                result.push_back(implicants[i]);
                for (size_t k = 0; k < fm.size(); k++) {
                    if (implicants[i].covers(getMaxtermValues(fm[k]))) covered[k] = true;
                }
                break;
            }
        }
    }
}

void SKNFMinimization::collectRemaining(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered) {
    const auto& fm = truth_table.getFalseMinterms();
    for (size_t i = 0; i < implicants.size(); i++) {
        bool needed = false;
        for (size_t j = 0; j < fm.size(); j++) {
            if (!covered[j] && implicants[i].covers(getMaxtermValues(fm[j]))) {
                needed = true;
                break;
            }
        }
        if (needed) {
            result.push_back(implicants[i]);
            for (size_t j = 0; j < fm.size(); j++) {
                if (implicants[i].covers(getMaxtermValues(fm[j]))) covered[j] = true;
            }
        }
    }
}

vector<Term> SKNFMinimization::removeRedundantImplicants(vector<Term>& implicants) {
    if (implicants.size() <= 1) return implicants;
    vector<Term> result;
    vector<bool> covered(truth_table.getFalseMinterms().size(), false);
    collectEssential(result, implicants, covered);
    collectRemaining(result, implicants, covered);
    return result;
}

void SKNFMinimization::printInitialSKNF() {
    const auto& fm = truth_table.getFalseMinterms();
    cout << "Initial SKNF (maxterms): ";
    for (size_t i = 0; i < fm.size(); i++) {
        if (i > 0) cout << " & ";
        cout << truth_table.getMaxtermString(fm[i]);
    }
    cout << "\n";
}

void SKNFMinimization::printGluingResult(int stage, const vector<Term>& terms) {
    const auto& vars = truth_table.getVariables();
    cout << "\nSKNF gluing result " << stage << ":\n  ";
    for (size_t i = 0; i < terms.size(); i++) {
        if (i > 0) cout << " & ";
        string termStr = maxtermToString(terms[i].values);
        cout << termStr;
    }
    cout << "\n";
}

vector<Term> SKNFMinimization::getPrimeImplicants() {
    const auto& fm = truth_table.getFalseMinterms();
    int n = truth_table.getVariables().size();
    vector<Term> terms;
    for (int idx : fm) {
        Term t(n);
        t.values = getMaxtermValues(idx);
        terms.push_back(t);
    }
    if (terms.empty()) return terms;
    bool changed;
    do { terms = glueOnce(terms, changed); } while (changed && terms.size() > 1);
    return terms;
}

vector<Term> SKNFMinimization::getPrimeImplicantsWithStages() {
    const auto& fm = truth_table.getFalseMinterms();
    const auto& vars = truth_table.getVariables();
    int n = vars.size();
    vector<Term> terms;
    for (int idx : fm) {
        Term t(n);
        t.values = getMaxtermValues(idx);
        terms.push_back(t);
    }
    if (terms.empty()) return terms;

    cout << "Initial maxterms (for SKNF):\n  ";
    for (size_t i = 0; i < terms.size(); i++) {
        if (i > 0) cout << " & ";
        cout << maxtermToString(terms[i].values);
    }
    cout << "\n";

    int stage = 1;
    bool changed;
    while (terms.size() > 1) {
        vector<Term> next = glueOnce(terms, changed);
        if (!changed) break;
        cout << "\nSKNF Gluing " << stage << ":\n";
        printGluingResult(stage, next);
        terms = next;
        stage++;
    }
    return terms;
}

void SKNFMinimization::minimizeQuine() {
    cout << "\n=== SKNF Minimization (Quine-McCluskey) ===\n";
    const auto& fm = truth_table.getFalseMinterms();
    const auto& vars = truth_table.getVariables();
    if (fm.empty()) {
        cout << "Function is identically 1\n";
        return;
    }

    printInitialSKNF();

    int n = vars.size();
    vector<Term> terms;
    for (int idx : fm) {
        Term t(n);
        t.values = getMaxtermValues(idx);
        terms.push_back(t);
    }

    cout << "\n=== SKNF Gluing stage ===\n";
    int stage = 1;
    bool changed;
    while (terms.size() > 1) {
        vector<Term> next = glueOnce(terms, changed);
        if (!changed) break;
        cout << "\nSKNF Gluing " << stage << ":\n";
        printGluingResult(stage, next);
        terms = next;
        stage++;
    }

    cout << "\n=== SKNF Removing redundant implicants ===\n";
    vector<Term> result = removeRedundantImplicants(terms);

    string sknf;
    for (size_t i = 0; i < result.size(); i++) {
        if (i > 0) sknf += " & ";
        sknf += maxtermToString(result[i].values);
    }

    if (sknf.empty()) sknf = "1";
    cout << "\nSKNF Minimization result: " << sknf << "\n";
}

// ==================== –¿—◊≈“ÕŒ-“¿¡À»◊Õ€… Ã≈“Œƒ (ÔÓ ÌÛÎˇÏ) ====================
int SKNFMinimization::findBestImplicant(const vector<vector<bool>>& cov, const vector<bool>& covered) {
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

vector<int> SKNFMinimization::selectMinCoverage(vector<vector<bool>>& coverage) {
    const auto& fm = truth_table.getFalseMinterms();
    vector<int> selected;
    vector<bool> covered(fm.size(), false);
    while (true) {
        int best = findBestImplicant(coverage, covered);
        if (best == -1) break;
        selected.push_back(best);
        for (size_t j = 0; j < fm.size(); j++) {
            if (coverage[best][j]) covered[j] = true;
        }
    }
    return selected;
}

void SKNFMinimization::printCoverageTable(const vector<Term>& implicants, const vector<int>& maxterms,
    const vector<vector<bool>>& cov) {
    cout << "     ";
    for (int idx : maxterms) cout << "M" << idx << " ";
    cout << "\n";
    cout << "     " << string(maxterms.size() * 3, '-') << "\n";
    for (size_t i = 0; i < implicants.size(); i++) {
        cout << "  " << char('A' + i) << ": ";
        for (size_t j = 0; j < maxterms.size(); j++) {
            cout << (cov[i][j] ? " X " : "   ");
        }
        cout << "\n";
    }
}

void SKNFMinimization::minimizeTable() {
    cout << "\n=== SKNF Calculation-table method ===\n";
    const auto& fm = truth_table.getFalseMinterms();
    const auto& vars = truth_table.getVariables();
    if (fm.empty()) {
        cout << "Function is identically 1\n";
        return;
    }

    cout << "=== Gluing stage (for zeros) ===\n";
    vector<Term> prime = getPrimeImplicantsWithStages();

    cout << "\n=== Prime implicants (maxterms) ===\n";
    for (size_t i = 0; i < prime.size(); i++) {
        cout << "  " << char('A' + i) << ": " << maxtermToString(prime[i].values);
        cout << " " << prime[i].getNumericString() << "\n";
    }

    cout << "\n=== Coverage table (for zeros) ===\n";
    vector<vector<bool>> cov(prime.size(), vector<bool>(fm.size(), false));
    for (size_t i = 0; i < prime.size(); i++) {
        for (size_t j = 0; j < fm.size(); j++) {
            if (prime[i].covers(getMaxtermValues(fm[j]))) cov[i][j] = true;
        }
    }
    printCoverageTable(prime, fm, cov);

    cout << "\n=== Selecting minimal coverage ===\n";
    vector<int> selected = selectMinCoverage(cov);
    string result;
    for (int i : selected) {
        if (!result.empty()) result += " & ";
        result += maxtermToString(prime[i].values);
    }
    cout << "\nMinimal SKNF: " << result << "\n";
}

// ==================== Ã≈“Œƒ  ¿–“  ¿–ÕŒ (ÔÓ ÌÛÎˇÏ) ====================
vector<vector<int>> SKNFMinimization::buildKarnaughMapForZeros() {
    int n = truth_table.getVariables().size();
    int layerBits, rowBits, colBits;
    splitVariables(n, layerBits, rowBits, colBits);

    int layers = (layerBits == 0) ? 1 : (1 << layerBits);
    int rows = (rowBits == 0) ? 1 : (1 << rowBits);
    int cols = (colBits == 0) ? 1 : (1 << colBits);

    vector<vector<int>> map(layers * rows, vector<int>(cols, 1)); // ÔÓ ÛÏÓÎ˜‡ÌË˛ 1 (Â‰ËÌËˆ˚)

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

        // ŒÚÏÂ˜‡ÂÏ ÌÛÎË
        if (truth_table.getTable()[i].back() == 0) {
            map[gray_layer * rows + gray_row][gray_col] = 0;
        }
    }
    return map;
}

void SKNFMinimization::printKarnaughMapForZeros(const vector<vector<int>>& map, int layerBits, int rowBits, int colBits) {
    int n = truth_table.getVariables().size();
    const auto& vars = truth_table.getVariables();

    if (n == 1) {
        cout << "Karnaugh map (for zeros):\n";
        cout << "0: " << map[0][0] << "\n";
        cout << "1: " << map[1][0] << "\n";
        return;
    }

    if (n == 2) {
        cout << "Karnaugh map (for zeros):\n";
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
        cout << "Karnaugh map for zeros (a \\ bc):\n";
        cout << "     00 01 11 10\n";
        cout << "   +------------\n";
        for (int r = 0; r < 2; r++) {
            cout << r << " |";
            for (int c = 0; c < 4; c++) {
                if (map[r][c] == 0) cout << " 0 ";
                else cout << " 1 ";
            }
            cout << "\n";
        }
        return;
    }

    if (n == 4) {
        cout << "Karnaugh map for zeros (ab \\ cd):\n";
        cout << "     00 01 11 10\n";
        cout << "   +---------------\n";
        string rowLabels[] = { "00", "01", "11", "10" };
        for (int r = 0; r < 4; r++) {
            cout << rowLabels[r] << " |";
            for (int c = 0; c < 4; c++) {
                if (map[r][c] == 0) cout << " 0 ";
                else cout << " 1 ";
            }
            cout << "\n";
        }
        return;
    }
}

string SKNFMinimization::minimizeByKarnaugh() {
    int n = truth_table.getVariables().size();
    if (n > 4) {
        return "Karnaugh map is recommended for up to 4 variables";
    }

    const auto& fm = truth_table.getFalseMinterms();
    if (fm.empty()) return "1";
    if ((int)fm.size() == (1 << n)) return "0";

    // œÓÒÚ‡ˇ ÏËÌËÏËÁ‡ˆËˇ: Ó·˙Â‰ËÌˇÂÏ ‚ÒÂ Ï‡ÍÒÚÂÏ˚
    string result;
    for (size_t i = 0; i < fm.size(); i++) {
        if (i > 0) result += " & ";
        result += truth_table.getMaxtermString(fm[i]);
    }
    return result;
}

void SKNFMinimization::minimizeKarnaugh() {
    cout << "\n=== SKNF Karnaugh map minimization ===\n";

    int n = truth_table.getVariables().size();
    if (n > 4) {
        cout << "Karnaugh map is recommended for up to 4 variables\n";
        return;
    }

    const auto& fm = truth_table.getFalseMinterms();
    if (fm.empty()) {
        cout << "Function is identically 1\n";
        return;
    }

    int layerBits, rowBits, colBits;
    splitVariables(n, layerBits, rowBits, colBits);

    vector<vector<int>> map = buildKarnaughMapForZeros();
    printKarnaughMapForZeros(map, layerBits, rowBits, colBits);

    cout << "\nZeros are marked as 0 in the map above\n";
    cout << "Group zeros to find minimal SKNF\n";

    string result = minimizeByKarnaugh();
    cout << "\nMinimal SKNF: " << result << "\n";
}

// ==================== ¬—≈ Ã≈“Œƒ€ —–¿«” ====================
void SKNFMinimization::minimizeAll() {
    minimizeQuine();
    minimizeTable();
    minimizeKarnaugh();
}