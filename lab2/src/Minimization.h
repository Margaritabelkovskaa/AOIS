#ifndef MINIMIZATION_H
#define MINIMIZATION_H

#include "CalculationMethod.h"
#include "CalculationTableMethod.h"
#include "KarnaughMethod.h"

class Minimization {
private:
    const TruthTable& truth_table;
    CalculationMethod calc;
    CalculationTableMethod calcTable;
    KarnaughMethod km;

public:
    Minimization(const TruthTable& tt);

    void calculateMethod();
    void calculationTableMethod();
    void karnaughMethod();

    // Добавляем этот метод
    vector<Term> getPrimeImplicants();
};

#endif