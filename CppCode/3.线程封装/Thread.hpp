#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>

template <class T>
using Func_t = std::function<void(T)>; // 参数类型为T 返回值为空的函数

template <class T>
class Thread
{
public:
    Thread(const std::string &tname, Func_t<T> func, T data)
        : _Tid(0), _ThreadName(tname), _IsRunning(false), _Func(func), _Data(data)
    {
    }

    static void *ThreadRoutine(void *args)
    {
        Thread *tp = static_cast<Thread *>(args);
        tp->_Func(tp->_Data); // 调用回调函数
        return nullptr;
    }

    ~Thread()
    {
    }

    bool Start() // 线程启动
    {
        int n = pthread_create(&_Tid, nullptr, ThreadRoutine, this);
        if (n == 0)
        {
            _IsRunning = true;
            return true;
        }
        else
            return false;
    }

    bool Join()
    {
        if (!_IsRunning) // 如果新线程已经结束运行了，那就没有等待的必要了
            return true;
        int n = pthread_join(_Tid, nullptr);
        if (n == 0)
        {
            _IsRunning = false;
            return true;
        }
        return false;
    }

    std::string GetThreadName()
    {
        return _ThreadName;
    }

    bool IsRunning()
    {
        return _IsRunning;
    }
private:
    pthread_t _Tid;          // 线程id 也可以用LWP来表示，主要是为了区分不同线程
    std::string _ThreadName; // 线程名称
    bool _IsRunning;         // 区分线程运行状态
    Func_t<T> _Func;         // 回调函数
    T _Data;
};