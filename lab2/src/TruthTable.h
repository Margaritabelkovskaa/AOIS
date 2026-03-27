#ifndef TRUTH_TABLE_H
#define TRUTH_TABLE_H

#include <vector>
#include <string>
#include "ExpressionParser.h"

using namespace std;

class TruthTable {
private:
    vector<char> variables;
    vector<vector<int>> table;
    vector<int> true_minterms;
    vector<int> false_minterms;

public:
    TruthTable(const ExpressionParser& parser);

    const vector<char>& getVariables() const { return variables; }
    const vector<vector<int>>& getTable() const { return table; }
    const vector<int>& getTrueMinterms() const { return true_minterms; }
    const vector<int>& getFalseMinterms() const { return false_minterms; }

    string getMintermString(int index) const;
    string getMaxtermString(int index) const;
    void print() const;
};

#endif
