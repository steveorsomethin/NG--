#include <core/ObservableValue.h>

using namespace ::testing;
using namespace NGPP::Core;

TEST(ObservableValue, Hacking)
{
    int derp = 0;

    ObservableValue<int> testObs = 20;
    ObservableValue<int> testObs2(0);

    testObs.PipeTo(&testObs2);

    testObs = 20;

    testObs2.SetReceiver([&derp](int value) {derp = value;}, true);

    ASSERT_EQ(20, derp);
}
