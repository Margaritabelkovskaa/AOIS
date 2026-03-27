#ifndef LOGIC_FUNCTION_H
#define LOGIC_FUNCTION_H

#include <string>
#include "ExpressionParser.h"
#include "TruthTable.h"

class LogicFunction {
private:
    string expression;
    ExpressionParser parser;
    TruthTable truth_table;

public:
    LogicFunction(const string& expr);

    void printTruthTable();
    string getSDNF();
    string getSKNF();
    string getNumericSDNF();
    string getNumericSKNF();
    string getIndexForm();

    // Нужно добавить эти методы
    void findDummyVariables();        
    void checkPostClasses();
    string getZhegalkinPolynomial();
    void booleanDifferentiation();
    void minimizeCalculation();
    void minimizeCalculationTable();
    void minimizeKarnaugh();

    const TruthTable& getTruthTable() const { return truth_table; }
};

#endif