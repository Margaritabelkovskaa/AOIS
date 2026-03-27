#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class ExpressionParser {
private:
    string expression;
    vector<char> variables;

    void extractVariables();
    vector<string> tokenize(const string& expr);
    int getPriority(const string& op);
    vector<string> toRPN(const vector<string>& tokens);
    bool evaluate(const vector<string>& rpn, const map<char, bool>& values);

public:
    ExpressionParser(const string& expr);
    const vector<char>& getVariables() const { return variables; }
    bool compute(const vector<int>& inputs) const;
};

#endif