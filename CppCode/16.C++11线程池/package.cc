#include <iostream>
#include <thread>
#include <future>
#include <memory>
#include "../logs/Xulog.h"

int Add(int num1, int num2)
{
    INFO("正在计算...");
    return num1 + num2;
}

int main()
{
    // std::packaged_task<int(int, int)> task(Add); // 包装函数
    // std::future<int> fu = task.get_future();     // 设置关联

    // 这样是不行的
    // std::async(std::launch::async, task, 114, 514);
    // std::thread(task, 114, 514);

    // task(11, 22); // 执行
    // std::shared_ptr<std::packaged_task<int(int, int)>>
    auto ptask = std::make_shared<std::packaged_task<int(int, int)>>(Add);
    std::future<int> fu = ptask->get_future();     // 设置关联

    std::thread thr([ptask]()
                    { (*ptask)(114, 514); });

    int sum = fu.get();
    INFO("结果是%d", sum);
    thr.join();
    return 0;
}