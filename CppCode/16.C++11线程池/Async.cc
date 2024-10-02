#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include "../logs/Xulog.h"

int Add(int num1, int num2)
{
    INFO("正在计算...");
    return num1 + num2;
}

int main()
{
    // std::async(func, ...)
    // std::async(policy, func, ...)
    INFO("异步任务执行");
    std::future<int> res = std::async(std::launch::async, Add, 114, 514);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    INFO("获取结果");
    int sum = res.get();
    INFO("结果是%d", sum);
    return 0;
}
