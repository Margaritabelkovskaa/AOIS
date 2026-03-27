#ifndef MINIMIZATION_H
#define MINIMIZATION_H

#include <vector>
#include <string>
#include "TruthTable.h"
#include "Term.h"

using namespace std;

class Minimization {
private:
    const TruthTable& truth_table;

    vector<Term> glueTerms(vector<Term>& terms);
    vector<Term> removeRedundantImplicants(vector<Term>& implicants);
    vector<int> selectMinCoverage(vector<vector<bool>>& coverage);
    vector<int> getMintermValues(int idx) const;
    int grayCode(int n);
    int grayToBinary(int n);

    // Методы для карт Карно
    void printKarnaughMap2Var(const vector<vector<int>>& k);
    void printKarnaughMap3Var(const vector<vector<int>>& k);
    void printKarnaughMap4Var(const vector<vector<int>>& k);

    vector<Term> findGroups2Var(const vector<vector<int>>& k);
    vector<Term> findGroups3Var(const vector<vector<int>>& k);
    vector<Term> findGroups4Var(const vector<vector<int>>& k);

    void minimizeKarnaugh1Var();
    void minimizeKarnaugh2Var();
    void minimizeKarnaugh3Var();
    void minimizeKarnaugh4Var();
    void minimizeKarnaugh5Var();

public:
    Minimization(const TruthTable& tt);

    void calculateMethod();
    void calculationTableMethod();
    void karnaughMethod();

    vector<Term> getPrimeImplicants();
    vector<Term> getPrimeImplicantsWithStages();
};

#endif