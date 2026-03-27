#ifndef ZHEGALKIN_POLYNOMIAL_H
#define ZHEGALKIN_POLYNOMIAL_H

#include <string>
#include "TruthTable.h"

class ZhegalkinPolynomial {
private:
    const TruthTable& truth_table;

public:
    ZhegalkinPolynomial(const TruthTable& tt);
    string getPolynomial() const;
};

#endif