#pragma once
#include <iostream>
#include <queue>
#include "LockGuard.hpp"

const int TOP = 5;

template <class T>
class BlockQueue
{
public:
    BlockQueue(int top = TOP)
        : _capacity(TOP)
    {
    }
    bool IsFull()
    {
        return _q.size() == _capacity;
    }
    bool IsEmpty()
    {
        return _q.size() == 0;
    }
    void Push(const T &in)
    {
        LockGuard lockguard(&_mutex);
        while (IsFull())
        {
            pthread_cond_wait(&_p_cond, &_mutex);
        }
        _q.push(in);
        pthread_cond_signal(&_c_cond);
    }
    void Pop(T *out)
    {
        LockGuard lockguard(&_mutex);
        while (IsEmpty())
        {
            pthread_cond_wait(&_c_cond, &_mutex);
        }
        *out = _q.front();
        _q.pop();
        pthread_cond_signal(&_p_cond);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_p_cond);
        pthread_cond_destroy(&_c_cond);
    }
private:
    std::queue<T> _q;
    int _capacity;          // 阻塞队列上限
    pthread_mutex_t _mutex; // 互斥锁
    pthread_cond_t _p_cond; // 生产者信号量
    pthread_cond_t _c_cond; // 消费者信号量
};