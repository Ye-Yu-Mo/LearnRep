#include <iostream>
#include <gtest/gtest.h>
#include <unordered_map>

#include "../logs/Xulog.h"

class MyEnvironment : public testing::Environment
{
public:
    virtual void SetUp() override
    {
        INFO("单元测试环境初始化!");
    }
    virtual void TearDown() override
    {
        INFO("单元测试结束销毁!");
    }
};

// TEST(MyEnvironment, test1)
// {
//     INFO("单元测试1");
// }

// TEST(MyEnvironment, test2)
// {
//     INFO("单元测试2");
// }

std::unordered_map<std::string, std::string> mp;
class MyMapTest : public testing::Environment
{
public:
    virtual void SetUp() override
    {
        INFO("单元测试环境初始化!");
        mp.insert(std::make_pair("hello","你好"));
    }
    virtual void TearDown() override
    {
        INFO("单元测试结束销毁!");
        mp.clear();
    }
};

TEST(MyMapTest, test1)
{
    INFO("单元测试2");
    ASSERT_EQ(mp.size(),1);
    mp.insert(std::make_pair("aa","AA"));
}

TEST(MyMapTest, test2)
{
    INFO("单元测试2");
    ASSERT_EQ(mp.size(),1);
    mp.erase("aa");
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new MyEnvironment);
    testing::AddGlobalTestEnvironment(new MyMapTest);
    RUN_ALL_TESTS();
    return 0;
}