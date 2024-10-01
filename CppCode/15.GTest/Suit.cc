#include <iostream>
#include <gtest/gtest.h>
#include <unordered_map>

#include "../logs/Xulog.h"

class MyTest : public testing::Test
{
public:
    static void SetUpTestCase()
    {
        INFO("初始化总环境!");
    }
    static void TearDownTestCase()
    {
        INFO("清理总环境");
    }

public:
    std::unordered_map<std::string, std::string> _mp;
};
TEST_F(MyTest, insert_test)
{
    _mp.insert(std::make_pair("hello", "你好"));
    _mp.insert(std::make_pair("aa", "AA"));
}
TEST_F(MyTest, size_test)
{
    ASSERT_EQ(_mp.size(), 2);
}

int main(int argc, char* argv[])
{   
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}