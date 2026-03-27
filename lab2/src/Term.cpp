#include "Term.h"

Term::Term(int n) : vars_count(n), values(n, -1) {}

bool Term::operator==(const Term& other) const {
    return values == other.values;
}

bool Term::covers(const vector<int>& minterm) const {
    for (int i = 0; i < vars_count; i++) {
        if (values[i] != -1 && values[i] != minterm[i]) {
            return false;
        }
    }
    return true;
}

string Term::toString(const vector<char>& var_names) const {
    if (isOne()) return "1";
    string result;
    bool first = true;
    for (int i = 0; i < vars_count; i++) {
        if (values[i] != -1) {
            if (!first) result += " & ";
            if (values[i] == 0) result += "!" + string(1, var_names[i]);
            else result += string(1, var_names[i]);
            first = false;
        }
    }
    return result;
}

string Term::getNumericString() const {
    string result = "(";
    for (int i = 0; i < vars_count; i++) {
        if (values[i] == -1) result += "X";
        else result += to_string(values[i]);
        if (i < vars_count - 1) result += ",";
    }
    result += ")";
    return result;
}

bool Term::isOne() const {
    for (int v : values) {
        if (v != -1) return false;
    }
    return true;
}

bool Term::canGlueWith(const Term& other, Term& result) const {
    int diff_count = 0;
    int diff_pos = -1;

    for (int i = 0; i < vars_count; i++) {
        if (values[i] != other.values[i]) {
            diff_count++;
            diff_pos = i;
            if (diff_count > 1) return false;
        }
    }

    if (diff_count == 1) {
        result = *this;
        result.values[diff_pos] = -1;
        return true;
    }
    return false;
}