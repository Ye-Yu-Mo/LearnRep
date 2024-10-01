#include <iostream>
#include <gtest/gtest.h>
#include "../logs/Xulog.h"

TEST(test, great_than)
{
    int age = 20;
    ASSERT_GT(age, 18);
    INFO("OK!");
}
int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}