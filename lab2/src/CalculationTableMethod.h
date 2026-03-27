#ifndef CALCULATION_TABLE_METHOD_H
#define CALCULATION_TABLE_METHOD_H

#include "TruthTable.h"
#include "Term.h"
#include "CalculationMethod.h"

using namespace std;

class CalculationTableMethod {
private:
    const TruthTable& truth_table;
    CalculationMethod calc;

    int findBestImplicant(const vector<vector<bool>>& cov, const vector<bool>& covered);
    vector<int> selectMinCoverage(vector<vector<bool>>& coverage);
    vector<int> getMintermValues(int idx) const;
    void printCoverageTable(const vector<Term>& implicants, const vector<int>& minterms,
        const vector<vector<bool>>& cov);

public:
    CalculationTableMethod(const TruthTable& tt);
    void minimize();
};

#endif
