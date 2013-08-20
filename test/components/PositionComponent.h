#include <components/PositionComponent.h>
#include <core/Entity.h>
#include <core/Input.h>

#include "../Mocks.h"

using namespace NGPP::Core;
using namespace NGPP::Components;
using namespace ::testing;

TEST(PositionComponent, OutputX)
{
    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    PositionComponent posComp = PositionComponent(&mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.x == 200.0f;}
            )
        )
    ).Times(1);

    posComp.SetX(200.0f);

    ASSERT_EQ(posComp.position.x, 200.0f);
}

TEST(PositionComponent, OutputY)
{
    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    PositionComponent posComp = PositionComponent(&mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.y == 200.0f;}
            )
        )
    ).Times(1);

    posComp.SetY(200.0f);

    ASSERT_EQ(posComp.position.y, 200.0f);
}

TEST(PositionComponent, OutputZ)
{
    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    PositionComponent posComp = PositionComponent(&mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.z == 200.0f;}
            )
        )
    ).Times(1);

    posComp.SetZ(200.0f);

    ASSERT_EQ(posComp.position.z, 200.0f);
}

TEST(PositionComponent, OutputAll)
{
    MockInput<Vector3> mockInput;
    MockOutput<Vector3> mockOutput;

    PositionComponent posComp = PositionComponent(&mockInput, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.x == 400.0f && value.y == 500.0f && value.z == 600.0f;}
            )
        )
    ).Times(1);

    posComp.SetPosition(Vector3(400.0f, 500.0f, 600.0f));

    ASSERT_EQ(posComp.position.x, 400.0f);
    ASSERT_EQ(posComp.position.y, 500.0f);
    ASSERT_EQ(posComp.position.z, 600.0f);
}

TEST(PositionComponent, InputOutput)
{
    Input<Vector3> input;
    MockOutput<Vector3> mockOutput;

    PositionComponent posComp = PositionComponent(&input, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.x == 400.0f && value.y == 500.0f && value.z == 600.0f;}
            )
        )
    ).Times(1);

    input.Receive(Vector3(400.0f, 500.0f, 600.0f));

    ASSERT_EQ(posComp.position.x, 400.0f);
    ASSERT_EQ(posComp.position.y, 500.0f);
    ASSERT_EQ(posComp.position.z, 600.0f);
}
