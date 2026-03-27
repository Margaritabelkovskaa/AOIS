#ifndef CALCULATION_METHOD_H
#define CALCULATION_METHOD_H

#include <vector>
#include <string>
#include "TruthTable.h"
#include "Term.h"

using namespace std;

class CalculationMethod {
private:
    const TruthTable& truth_table;

    vector<int> getMintermValues(int idx) const;
    bool tryGlue(const Term& a, const Term& b, Term& result);
    void addUnique(vector<Term>& list, const Term& term);
    void removeDuplicates(vector<Term>& terms);
    bool isEssential(const Term& imp, const vector<Term>& all, int minterm);
    void collectEssential(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered);
    void collectRemaining(vector<Term>& result, vector<Term>& implicants, vector<bool>& covered);
    void printInitialSDNF();
    void printNumericSets();
    void printGluingResult(int stage, const vector<Term>& terms);

public:
    CalculationMethod(const TruthTable& tt);

    vector<Term> glueOnce(vector<Term>& terms, bool& changed);
    vector<Term> removeRedundantImplicants(vector<Term>& implicants);
    void minimize();
    vector<Term> getPrimeImplicants();
    vector<Term> getPrimeImplicantsWithStages();
};

#endif