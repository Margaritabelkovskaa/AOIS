#ifndef KARNAUGH_METHOD_H
#define KARNAUGH_METHOD_H

#include "TruthTable.h"
#include "Term.h"
#include "CalculationMethod.h"

using namespace std;

class KarnaughMethod {
private:
    const TruthTable& truth_table;
    CalculationMethod calc;

    // Универсальные методы
    int grayCode(int n);
    int grayToBinary(int n);
    vector<int> getMintermValues(int idx) const;
    string groupsToString(const vector<Term>& groups);

    // Разбиение переменных
    void splitVariables(int n, int& layerBits, int& rowBits, int& colBits);

    // Построение и вывод карты (универсальные)
    vector<vector<int>> buildKarnaughMap();
    void printKarnaughMap(const vector<vector<int>>& map, int layerBits, int rowBits, int colBits);

    // Создание термов из минтермов
    vector<Term> createTermsFromMinterms();

    // Минимизация через расчетный метод
    vector<Term> minimizeByCalculation();

public:
    KarnaughMethod(const TruthTable& tt);
    void minimize();
};

#endif