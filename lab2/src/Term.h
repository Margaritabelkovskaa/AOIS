#ifndef TERM_H
#define TERM_H

#include <vector>
#include <string>

using namespace std;

struct Term {
    vector<int> values; // делаем public
    int vars_count;

    Term(int n);
    bool operator==(const Term& other) const;
    bool covers(const vector<int>& minterm) const;
    string toString(const vector<char>& var_names) const;
    string getNumericString() const;
    bool isOne() const;
    bool canGlueWith(const Term& other, Term& result) const;
};

#endif
