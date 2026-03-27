#ifndef POST_CLASSES_H
#define POST_CLASSES_H

#include "TruthTable.h"

class PostClasses {
private:
    const TruthTable& truth_table;

public:
    PostClasses(const TruthTable& tt);
    void checkAll();
    bool isT0() const;
    bool isT1() const;
    bool isS() const;
    bool isM() const;
    bool isL() const;
};

#endif

