#include <gtest/gtest.h>
#include "BaseFunctions.h"

TEST(LerpTest, BasicAssertion)
{
	EXPECT_DOUBLE_EQ(lerpFunc(200, 100, 0), 200);
	EXPECT_DOUBLE_EQ(lerpFunc(200, 100, 1), 100);
	EXPECT_DOUBLE_EQ(lerpFunc(200, 100, 0.5f), 150);
}