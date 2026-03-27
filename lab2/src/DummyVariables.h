#ifndef DUMMY_VARIABLES_H
#define DUMMY_VARIABLES_H

#include "TruthTable.h"
#include <vector>
#include <string>

using namespace std;

class DummyVariables {
private:
    const TruthTable& truth_table;
    vector<bool> isDummy;
    
    void computeDummyVariables();

public:
    DummyVariables(const TruthTable& tt);
    bool isVariableDummy(int idx) const;
    
    vector<char> getDummyVariables() const;
    
    vector<char> getEssentialVariables() const;

    bool hasDummyVariables() const;
   
    void print() const;
    
    int getDummyCount() const;

    int getEssentialCount() const;
};

#endif
