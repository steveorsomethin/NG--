#include <components/TweenComponent.h>
#include <core/Entity.h>
#include <core/Input.h>
#include <core/Time.h>

#include "../Mocks.h"

using namespace NGPP::Core;
using namespace NGPP::Components;
using namespace ::testing;

TEST(TweenComponent, OutputX)
{
    Time time = {0, 0, 16, 0.0f};

    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    TweenComponent tweenComp = TweenComponent(100, &mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.x == 16.0f;}
            )
        )
    ).Times(1);

    tweenComp.Tween(Vector3(100.0f, 0.0f, 0.0f));

    tweenComp.Tick(time);

    ASSERT_EQ(tweenComp.value.x, 16.0f);
}

TEST(TweenComponent, OutputY)
{
    Time time = {0, 0, 16, 0.0f};

    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    TweenComponent tweenComp = TweenComponent(100, &mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.y == 32.0f;}
            )
        )
    ).Times(1);

    tweenComp.Tween(Vector3(0.0f, 200.0f, 00.0f));

    tweenComp.Tick(time);

    ASSERT_EQ(tweenComp.value.y, 32.0f);
}

TEST(TweenComponent, OutputZ)
{
    Time time = {0, 0, 16, 0.0f};

    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    TweenComponent tweenComp = TweenComponent(100, &mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.z == 48.0f;}
            )
        )
    ).Times(1);

    tweenComp.Tween(Vector3(0.0f, 0.0f, 300.0f));

    tweenComp.Tick(time);

    ASSERT_EQ(tweenComp.value.z, 48.0f);
}

TEST(TweenComponent, OutputAll)
{
    Time time = {0, 0, 16, 0.0f};

    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    TweenComponent tweenComp = TweenComponent(100, &mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.x == 16.0f && value.y == 32.0f && value.z == 48.0f;}
            )
        )
    ).Times(1);

    tweenComp.Tween(Vector3(100.0f, 200.0f, 300.0f));

    tweenComp.Tick(time);

    ASSERT_EQ(tweenComp.value.x, 16.0f);
    ASSERT_EQ(tweenComp.value.y, 32.0f);
    ASSERT_EQ(tweenComp.value.z, 48.0f);
}
