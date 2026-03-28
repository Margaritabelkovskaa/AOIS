#include <gtest/gtest.h>
#include "LogicFunction.h"
#include "PostClasses.h"

using namespace std;


TEST(PostClassesTest, AND_T0_T1) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    PostClasses post(tt);

    EXPECT_TRUE(post.isT0());
    EXPECT_TRUE(post.isT1());
    EXPECT_FALSE(post.isS());
    EXPECT_TRUE(post.isM());
    EXPECT_FALSE(post.isL());
}

TEST(PostClassesTest, OR_T0_T1) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    PostClasses post(tt);

    EXPECT_TRUE(post.isT0());
    EXPECT_TRUE(post.isT1());
    EXPECT_FALSE(post.isS());
    EXPECT_TRUE(post.isM());
    EXPECT_FALSE(post.isL());
}

TEST(PostClassesTest, NOT_T0_T1) {
    LogicFunction logic("!a");
    const TruthTable& tt = logic.getTruthTable();
    PostClasses post(tt);

    EXPECT_FALSE(post.isT0());
    EXPECT_FALSE(post.isT1());
    EXPECT_TRUE(post.isS());
    EXPECT_FALSE(post.isM());
    EXPECT_TRUE(post.isL());
}

TEST(PostClassesTest, Implication_M) {
    LogicFunction logic("a->b");
    const TruthTable& tt = logic.getTruthTable();
    PostClasses post(tt);

    EXPECT_FALSE(post.isT0());
    EXPECT_TRUE(post.isT1());
    EXPECT_FALSE(post.isS());
    EXPECT_FALSE(post.isM());
    EXPECT_FALSE(post.isL());
}

TEST(PostClassesTest, XOR_S) {
    LogicFunction logic("a~b");
    const TruthTable& tt = logic.getTruthTable();
    PostClasses post(tt);

    EXPECT_FALSE(post.isT0());
    EXPECT_TRUE(post.isT1());
    EXPECT_FALSE(post.isS());
    EXPECT_FALSE(post.isM());
    EXPECT_TRUE(post.isL());
}

TEST(PostClassesTest, CheckAll) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    PostClasses post(tt);

    testing::internal::CaptureStdout();
    post.checkAll();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("T0") != string::npos);
}

TEST(PostClassesTest, AllMethods) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    PostClasses post(tt);

    EXPECT_TRUE(post.isT0());
    EXPECT_TRUE(post.isT1());
    EXPECT_FALSE(post.isS());
    EXPECT_TRUE(post.isM());
    EXPECT_FALSE(post.isL());

    testing::internal::CaptureStdout();
    post.checkAll();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("T0") != string::npos);
}
