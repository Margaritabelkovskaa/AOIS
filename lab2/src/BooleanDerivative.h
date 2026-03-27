#ifndef BOOLEAN_DERIVATIVE_H
#define BOOLEAN_DERIVATIVE_H

#include "TruthTable.h"

class BooleanDerivative {
private:
    const TruthTable& truth_table;

public:
    BooleanDerivative(const TruthTable& tt);
    void computeAll() const;
};

#endif
