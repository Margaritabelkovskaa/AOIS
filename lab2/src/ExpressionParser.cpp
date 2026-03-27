#include "ExpressionParser.h"
#include <stack>
#include <cctype>
#include <algorithm>
#include <cmath>

using namespace std;

const set<char> VALID_VARS = { 'a', 'b', 'c', 'd', 'e' };

ExpressionParser::ExpressionParser(const string& expr) : expression(expr) {
    extractVariables();
}

void ExpressionParser::extractVariables() {
    set<char> vars;
    for (char c : expression) {
        if (VALID_VARS.find(c) != VALID_VARS.end()) {
            vars.insert(c);
        }
    }
    variables = vector<char>(vars.begin(), vars.end());
    sort(variables.begin(), variables.end());
}

vector<string> ExpressionParser::tokenize(const string& expr) {
    vector<string> tokens;
    string current;
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (c == ' ') continue;
        if (c == '!' || c == '&' || c == '|' || c == '(' || c == ')' || c == '~') {
            if (!current.empty()) { tokens.push_back(current); current.clear(); }
            tokens.push_back(string(1, c));
        }
        else if (c == '-' && i + 1 < expr.length() && expr[i + 1] == '>') {
            if (!current.empty()) { tokens.push_back(current); current.clear(); }
            tokens.push_back("->");
            i++;
        }
        else if (VALID_VARS.find(c) != VALID_VARS.end()) {
            current += c;
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

int ExpressionParser::getPriority(const string& op) {
    if (op == "!") return 4;
    if (op == "&") return 3;
    if (op == "|") return 2;
    if (op == "->") return 1;
    if (op == "~") return 1;
    return 0;
}

vector<string> ExpressionParser::toRPN(const vector<string>& tokens) {
    vector<string> output;
    stack<string> operators;
    for (const string& token : tokens) {
        if (token == "(") {
            operators.push(token);
        }
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop();
        }
        else if (token == "!" || token == "&" || token == "|" || token == "->" || token == "~") {
            while (!operators.empty() && operators.top() != "(" &&
                getPriority(operators.top()) >= getPriority(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
        else {
            output.push_back(token);
        }
    }
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }
    return output;
}

bool ExpressionParser::evaluate(const vector<string>& rpn, const map<char, bool>& values) {
    stack<bool> st;
    for (const string& token : rpn) {
        if (token == "!") {
            bool a = st.top(); st.pop();
            st.push(!a);
        }
        else if (token == "&") {
            bool b = st.top(); st.pop();
            bool a = st.top(); st.pop();
            st.push(a && b);
        }
        else if (token == "|") {
            bool b = st.top(); st.pop();
            bool a = st.top(); st.pop();
            st.push(a || b);
        }
        else if (token == "->") {
            bool b = st.top(); st.pop();
            bool a = st.top(); st.pop();
            st.push(!a || b);
        }
        else if (token == "~") {
            bool b = st.top(); st.pop();
            bool a = st.top(); st.pop();
            st.push(a == b);
        }
        else {
            char var = token[0];
            auto it = values.find(var);
            if (it != values.end()) {
                st.push(it->second);
            }
        }
    }
    return st.top();
}

bool ExpressionParser::compute(const vector<int>& inputs) const {
    map<char, bool> values;
    int n = variables.size();
    for (int j = 0; j < n; j++) {
        values[variables[j]] = inputs[j];
    }
    // Создаем временную копию для вызова неконстантных методов
    vector<string> tokens = const_cast<ExpressionParser*>(this)->tokenize(expression);
    vector<string> rpn = const_cast<ExpressionParser*>(this)->toRPN(tokens);
    return const_cast<ExpressionParser*>(this)->evaluate(rpn, values);
}