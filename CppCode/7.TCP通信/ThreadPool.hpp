#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <functional>
#include "Thread.hpp"
#include "LockGuard.hpp"

namespace ThreadNs
{
    static const int defaultnum = 8;
    class ThreadData
    {
    public:
        ThreadData(const std::string name)
            : threadname(name)
        {
        }
        ~ThreadData() {}

    public:
        std::string threadname;
    };

    template <class T>
    class ThreadPool
    {
    private:
        ThreadPool(int thread_num = defaultnum)
            : _thread_num(thread_num)
        {
            pthread_mutex_init(&_mutex, nullptr);
            pthread_cond_init(&_cond, nullptr);
            // 构建线程
            for (int i = 0; i < _thread_num; i++)
            {
                std::string threadname = "thread-";
                threadname += std::to_string(i);
                ThreadData td(threadname);
                _threads.emplace_back(threadname, std::bind(&ThreadPool<T>::ThreadRun, this, std::placeholders::_1), td);
                std::cout << threadname.c_str() << "已经创建\n";
            }
        }
        ThreadPool(const ThreadPool<T> &tp) = delete;
        const ThreadPool<T> &operator=(const ThreadPool<T>) = delete;

    public:
        static ThreadPool<T> *GetInstance()
        {
            LockGuard lockguard(&sig_lock);
            if (instance == nullptr)
            {
                std::cout << "线程池创建单例成功";
                instance = new ThreadPool<T>();
            }
            return instance;
        }
        bool Start()
        {
            for (auto &thread : _threads)
            {
                thread.Start();
                std::cout << thread.GetThreadName().c_str() << "正在运行\n";
            }
            return true;
        }
        void ThreadWait(const ThreadData &td)
        {
            std::cout << "当前无任务，" << td.threadname.c_str() << "正在等待\n";
            pthread_cond_wait(&_cond, &_mutex);
        }
        void ThreadWakeup()
        {
            pthread_cond_signal(&_cond);
        }
        void ThreadRun(ThreadData &td)
        {
            while (true)
            {
                T t;
                {
                    LockGuard lockguard(&_mutex);
                    while (_q.empty())
                    {
                        ThreadWait(td);
                        std::cout << td.threadname.c_str() << "已经唤醒\n";
                    }
                    t = _q.front();
                }
                t();
            }
        }
        void Push(T in)
        {
            LockGuard lockguard(&_mutex);
            _q.push(in);
            ThreadWakeup();
        }
        ~ThreadPool()
        {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_cond);
        }
        void Wait()
        {
            for (auto &thread : _threads)
                thread.Join();
        }

    private:
        std::queue<T> _q;
        std::vector<Thread<ThreadData>> _threads;
        int _thread_num;
        pthread_mutex_t _mutex;
        pthread_cond_t _cond;

        static ThreadPool<T> *instance;
        static pthread_mutex_t sig_lock;
    };

    template <class T>
    ThreadPool<T> *ThreadPool<T>::instance = nullptr;
    template <class T>
    pthread_mutex_t ThreadPool<T>::sig_lock = PTHREAD_MUTEX_INITIALIZER;
};