#include "LogicFunction.h"
#include "PostClasses.h"
#include "ZhegalkinPolynomial.h"
#include "BooleanDerivative.h"
#include "Minimization.h"
#include "DummyVariables.h"
#include <iostream>

using namespace std;

LogicFunction::LogicFunction(const string& expr) : expression(expr), parser(expr), truth_table(parser) {}

void LogicFunction::printTruthTable() {
    truth_table.print();
}

string LogicFunction::getSDNF() {
    const auto& true_minterms = truth_table.getTrueMinterms();
    if (true_minterms.empty()) return "0";
    string result;
    for (size_t i = 0; i < true_minterms.size(); i++) {
        if (i > 0) result += " | ";
        result += "(" + truth_table.getMintermString(true_minterms[i]) + ")";
    }
    return result;
}

string LogicFunction::getSKNF() {
    const auto& false_minterms = truth_table.getFalseMinterms();
    if (false_minterms.empty()) return "1";
    string result;
    for (size_t i = 0; i < false_minterms.size(); i++) {
        if (i > 0) result += " & ";
        result += truth_table.getMaxtermString(false_minterms[i]);
    }
    return result;
}

string LogicFunction::getNumericSDNF() {
    const auto& true_minterms = truth_table.getTrueMinterms();
    string result = "СДНФ: ";
    for (size_t i = 0; i < true_minterms.size(); i++) {
        if (i > 0) result += ", ";
        result += to_string(true_minterms[i]);
    }
    return result;
}

string LogicFunction::getNumericSKNF() {
    const auto& false_minterms = truth_table.getFalseMinterms();
    string result = "СКНФ: ";
    for (size_t i = 0; i < false_minterms.size(); i++) {
        if (i > 0) result += ", ";
        result += to_string(false_minterms[i]);
    }
    return result;
}

string LogicFunction::getIndexForm() {
    string result;
    for (const auto& row : truth_table.getTable()) {
        result += to_string(row.back());
    }
    return result;
}

void LogicFunction::findDummyVariables() {
    DummyVariables dummy(truth_table);
    dummy.print();
}

void LogicFunction::checkPostClasses() {
    PostClasses post(truth_table);
    post.checkAll();
}

string LogicFunction::getZhegalkinPolynomial() {
    ZhegalkinPolynomial poly(truth_table);
    return poly.getPolynomial();
}

void LogicFunction::booleanDifferentiation() {
    BooleanDerivative deriv(truth_table);
    deriv.computeAll();
}

void LogicFunction::minimizeCalculation() {
    Minimization min(truth_table);
    min.calculateMethod();
}

void LogicFunction::minimizeCalculationTable() {
    Minimization min(truth_table);
    min.calculationTableMethod();
}

void LogicFunction::minimizeKarnaugh() {
    Minimization min(truth_table);
    min.karnaughMethod();
}
