#pragma once
#include "Thread.hpp"
#include "LockGuard.hpp"
#include "write.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <unistd.h>

static const int THREAD_NUM = 5;
Xulog::Formatter fmt;

class ThreadData
{
public:
    ThreadData(const std::string &name)
        : threadname(name)
    {
    }
    ~ThreadData() {}
    std::string threadname;
};

template <class T>
class ThreadPool
{
public:
    ThreadPool(int thread_num = THREAD_NUM)
        : _thread_num(thread_num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        // 构建线程
        for (int i = 0; i < _thread_num; i++)
        {
            std::string threadname = "thread-";
            threadname += std::to_string(i + 1);
            ThreadData td(threadname);
            _threads.emplace_back(threadname, std::bind(&ThreadPool::ThreadRun, this, std::placeholders::_1), td);
            Xulog::LogMsg msg(Xulog::LogLevel::value::DEBUG, 0, "default", td.threadname.c_str(), "线程已经启动");
            WriteLog(msg, fmt);
        }
    }
    void ThreadRun(ThreadData &td)
    {
        while (true)
        {
            // 取到任务
            T task;
            {
                LockGuard lockguard(&_mutex);
                while (_task_que.empty())
                {
                    ThreadWait(td);
                }
                task = _task_que.front();
                _task_que.pop();
            }
            // 处理任务
            task();
            Xulog::LogMsg msg(Xulog::LogLevel::value::DEBUG, 0, "default", td.threadname.c_str(), "任务已经完成,结果是:" + task.PrintResult());
            WriteLog(msg, fmt);
            sleep(1);
        }
    }
    void ThreadWait(ThreadData &td)
    {
        Xulog::LogMsg msg(Xulog::LogLevel::value::INFO, 0, "default", "root", td.threadname + "休眠了");
        WriteLog(msg, fmt);

        pthread_cond_wait(&_cond, &_mutex);
    }
    void ThreadWeakUp()
    {
        pthread_cond_signal(&_cond);
    }
    bool Start()
    {
        // 启动线程
        for (auto &thread : _threads)
        {
            thread.Start();
        }
        return true;
    }
    void Push(const T &in)
    {
        Xulog::LogMsg msg(Xulog::LogLevel::value::DEBUG, 0, "default", "root", "生成了一个任务,任务是:" + in.PrintTask());
        WriteLog(msg, fmt);

        LockGuard lockguard(&_mutex);
        _task_que.push(in);
        ThreadWeakUp();
    }
    void Wait() // 调试方法
    {
        for (auto &thread : _threads)
        {
            thread.Join();
        }
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    std::queue<T> _task_que;
    std::vector<Thread<ThreadData>> _threads;
    int _thread_num;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};
