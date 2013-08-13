#include <components/ScaleComponent.h>
#include <core/Entity.h>
#include <core/Input.h>

#include "../Mocks.h"

using namespace NGPP::Core;
using namespace NGPP::Components;
using namespace ::testing;

TEST(ScaleComponent, OutputAll) {
    Input<Vector3> input;
    MockOutput<Vector3> mockOutput;

    ScaleComponent scaleComp = ScaleComponent(3.0f, &input, &mockOutput);

    EXPECT_CALL(mockOutput,
        Send(
            Truly(
                [](Vector3 value){return value.x == 300.0f && value.y == 600.0f && value.z == 900.0f;}
            )
        )
    ).Times(1);

    input.Receive(Vector3(100.0f, 200.0f, 300.0f));
}
