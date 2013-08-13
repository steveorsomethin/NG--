#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "components/PositionComponent.h"
#include "components/ScaleComponent.h"
#include "components/TweenComponent.h"

int main(int argc, char * argv[])
{
    InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}