#include <gtest/gtest.h>
#include "LogicFunction.h"
#include "BooleanDerivative.h"

using namespace std;


TEST(BooleanDerivativeTest, ComputeAllAND) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    BooleanDerivative deriv(tt);

    testing::internal::CaptureStdout();
    deriv.computeAll();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("df/da:") != string::npos);
    EXPECT_TRUE(output.find("df/db:") != string::npos);
}

TEST(BooleanDerivativeTest, ComputeAllOR) {
    LogicFunction logic("a|b");
    const TruthTable& tt = logic.getTruthTable();
    BooleanDerivative deriv(tt);

    testing::internal::CaptureStdout();
    deriv.computeAll();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("df/da:") != string::npos);
    EXPECT_TRUE(output.find("df/db:") != string::npos);
}

TEST(BooleanDerivativeTest, ComputeAllNOT) {
    LogicFunction logic("!a");
    const TruthTable& tt = logic.getTruthTable();
    BooleanDerivative deriv(tt);

    testing::internal::CaptureStdout();
    deriv.computeAll();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("df/da:") != string::npos);
}

TEST(BooleanDerivativeTest, AllMethods) {
    LogicFunction logic("a&b");
    const TruthTable& tt = logic.getTruthTable();
    BooleanDerivative deriv(tt);

    testing::internal::CaptureStdout();
    deriv.computeAll();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("df/da:") != string::npos);
    EXPECT_TRUE(output.find("df/db:") != string::npos);

    LogicFunction logic2("a&b&c");
    const TruthTable& tt2 = logic2.getTruthTable();
    BooleanDerivative deriv2(tt2);

    testing::internal::CaptureStdout();
    deriv2.computeAll();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("df/da:") != string::npos);
    EXPECT_TRUE(output.find("df/db:") != string::npos);
    EXPECT_TRUE(output.find("df/dc:") != string::npos);
}
