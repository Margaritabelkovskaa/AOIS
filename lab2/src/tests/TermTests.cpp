#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "Term.h"

using namespace std;

TEST(TermTest, Constructor) {
    Term t(3);
    EXPECT_EQ(t.vars_count, 3);
    EXPECT_EQ(t.values.size(), 3);
    for (int v : t.values) {
        EXPECT_EQ(v, -1);
    }
}

TEST(TermTest, IsOne) {
    Term t(3);
    EXPECT_TRUE(t.isOne());

    t.values = {0, 1, -1};
    EXPECT_FALSE(t.isOne());

    t.values = {1, 1, 1};
    EXPECT_FALSE(t.isOne());
}

TEST(TermTest, Covers) {
    Term t(3);
    t.values = {0, 1, -1};

    vector<int> minterm1 = {0, 1, 0};
    vector<int> minterm2 = {0, 1, 1};
    vector<int> minterm3 = {1, 1, 0};

    EXPECT_TRUE(t.covers(minterm1));
    EXPECT_TRUE(t.covers(minterm2));
    EXPECT_FALSE(t.covers(minterm3));
}

TEST(TermTest, CoversWithX) {
    Term t(3);
    t.values = {-1, -1, 1};

    vector<int> minterm1 = {0, 0, 1};
    vector<int> minterm2 = {0, 1, 1};
    vector<int> minterm3 = {1, 0, 1};
    vector<int> minterm4 = {1, 1, 1};

    EXPECT_TRUE(t.covers(minterm1));
    EXPECT_TRUE(t.covers(minterm2));
    EXPECT_TRUE(t.covers(minterm3));
    EXPECT_TRUE(t.covers(minterm4));
    EXPECT_FALSE(t.covers({0, 0, 0}));
}

TEST(TermTest, ToString) {
    vector<char> vars = {'a', 'b', 'c'};
    Term t(3);
    t.values = {0, 1, -1};
    EXPECT_EQ(t.toString(vars), "!a & b");

    t.values = {1, 0, 1};
    EXPECT_EQ(t.toString(vars), "a & !b & c");

    t.values = {1, -1, -1};
    EXPECT_EQ(t.toString(vars), "a");

    t.values = {-1, -1, -1};
    EXPECT_EQ(t.toString(vars), "1");
}

TEST(TermTest, GetNumericString) {
    Term t(3);
    t.values = {0, 1, -1};
    EXPECT_EQ(t.getNumericString(), "(0,1,X)");

    t.values = {1, 0, 1};
    EXPECT_EQ(t.getNumericString(), "(1,0,1)");

    t.values = {-1, -1, -1};
    EXPECT_EQ(t.getNumericString(), "(X,X,X)");
}

TEST(TermTest, Equality) {
    Term t1(3);
    Term t2(3);
    t1.values = {0, 1, -1};
    t2.values = {0, 1, -1};
    EXPECT_TRUE(t1 == t2);

    t2.values = {0, 1, 0};
    EXPECT_FALSE(t1 == t2);
}

TEST(TermTest, CanGlueWith) {
    Term t1(3);
    Term t2(3);
    Term result(3);

    t1.values = {0, 1, 0};
    t2.values = {0, 1, 1};

    EXPECT_TRUE(t1.canGlueWith(t2, result));
    EXPECT_EQ(result.values[0], 0);
    EXPECT_EQ(result.values[1], 1);
    EXPECT_EQ(result.values[2], -1);
}

TEST(TermTest, CanGlueWithMultipleDiff) {
    Term t1(3);
    Term t2(3);
    Term result(3);

    t1.values = {0, 1, 0};
    t2.values = {1, 0, 1};

    EXPECT_FALSE(t1.canGlueWith(t2, result));
}

TEST(TermTest, CanGlueWithX) {
    Term t1(3);
    Term t2(3);
    Term result(3);

    t1.values = {0, -1, 0};
    t2.values = {0, 1, 0};

    EXPECT_TRUE(t1.canGlueWith(t2, result));
    EXPECT_EQ(result.values[0], 0);
    EXPECT_EQ(result.values[1], -1);
    EXPECT_EQ(result.values[2], 0);
}
