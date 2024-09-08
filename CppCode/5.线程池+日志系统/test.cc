#include "util.hpp"
#include "level.hpp"
#include "format.hpp"
#include "write.hpp"
#include "ThreadPool.hpp"
#include "Task.hpp"


int main()
{
    // 测试多向写入模块
    // Xulog::LogMsg msg(Xulog::LogLevel::value::ERROR, 124, "main.cc", "root", "格式化功能测试");
    // WriteLog(msg, fmt);            // 默认输出到屏幕
    // WriteLog(msg, fmt, OneFile);   // 输出到单个文件
    // WriteLog(msg, fmt, ClassFile); // 分类输出

    std::unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>());
    tp->Start();
    srand(time(nullptr));

    while (true)
    {
        int x = rand() % 100;
        usleep(1145);
        int y = rand() % 100;
        usleep(1145);
        char op = operators[rand() % operators.size()];

        Task task(x, y, op);
        tp->Push(task);
        sleep(1);
    }

    tp->Wait();
    return 0;
}