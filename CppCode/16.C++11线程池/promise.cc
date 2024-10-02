#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include "../logs/Xulog.h"

void Add(int num1, int num2, std::promise<int> &prom)
{
    INFO("正在计算...");
    prom.set_value(num1+num2);
}
int main()
{
    std::promise<int> prom; // 创建promise对象
    std::future<int> fu = prom.get_future(); // 关联future对象

    std::thread thr(Add, 114, 514, std::ref(prom));
    // 异步线程中对promise对象进行设置值

    int res = fu.get();
    // 从future中获取数据
    INFO("结果是%d", res);
    thr.join();
    return 0;
}