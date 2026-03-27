#include "Minimization.h"

Minimization::Minimization(const TruthTable& tt)
    : truth_table(tt), calc(tt), calcTable(tt), km(tt) {
}

void Minimization::calculateMethod() {
    calc.minimize();
}

void Minimization::calculationTableMethod() {
    calcTable.minimize();
}

void Minimization::karnaughMethod() {
    km.minimize();
}

// Реализация метода
vector<Term> Minimization::getPrimeImplicants() {
    return calc.getPrimeImplicants();
}