#ifndef SKNF_MINIMIZATION_H
#define SKNF_MINIMIZATION_H

#include "TruthTable.h"
#include "Term.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

class SKNFMinimization {
private:
    const TruthTable& truth_table;

    // ========== ОБЩИЕ ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ==========
    vector<int> getMaxtermValues(int idx) const;
    int grayCode(int n);
    int grayToBinary(int n);
    void splitVariables(int n, int& layerBits, int& rowBits, int& colBits);
    string maxtermToString(const vector<int>& values) const;

    // ========== МЕТОД КВАЙНА-МАК-КЛАСКИ (по нулям) ==========
    bool tryGlueMaxterm(const Term& a, const Term& b, Term& result);
    void addUnique(vector<Term>& list, const Term& term);
    void removeDuplicates(vector<Term>& terms);
    bool isEssentialMaxterm(const Term& imp, const vector<Term>& all, int maxterm);
    void collectEssential(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered);
    void collectRemaining(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered);
    vector<Term> glueOnce(vector<Term>& terms, bool& changed);
    vector<Term> removeRedundantImplicants(vector<Term>& implicants);
    void printInitialSKNF();
    void printGluingResult(int stage, const vector<Term>& terms);

    // ========== РАСЧЕТНО-ТАБЛИЧНЫЙ МЕТОД (по нулям) ==========
    int findBestImplicant(const vector<vector<bool>>& cov, const vector<bool>& covered);
    vector<int> selectMinCoverage(vector<vector<bool>>& coverage);
    void printCoverageTable(const vector<Term>& implicants, const vector<int>& maxterms,
        const vector<vector<bool>>& cov);

    // ========== МЕТОД КАРТ КАРНО (по нулям) ==========
    vector<vector<int>> buildKarnaughMapForZeros();
    void printKarnaughMapForZeros(const vector<vector<int>>& map, int layerBits, int rowBits, int colBits);
    string minimizeByKarnaugh();

public:
    SKNFMinimization(const TruthTable& tt);

    // Метод Квайна-Мак-Класки (расчетный по нулям)
    vector<Term> getPrimeImplicants();
    vector<Term> getPrimeImplicantsWithStages();
    void minimizeQuine();

    // Расчетно-табличный метод (по нулям)
    void minimizeTable();

    // Метод карт Карно (по нулям)
    void minimizeKarnaugh();

    // Все методы сразу
    void minimizeAll();
};

#endif