#pragma once

#include <pthread.h>

class Mutex // 互斥锁，具体的锁在外部定义，传进来进行调用
{
public:
    Mutex(pthread_mutex_t *lock)
        : _lock(lock)
    {
    }
    ~Mutex() {}
    void Lock()
    {
        pthread_mutex_lock(_lock);
    }
    void UnLock()
    {
        pthread_mutex_unlock(_lock);
    }

private:
    pthread_mutex_t *_lock;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t *lock)
        : _mutex(lock)
    {
        _mutex.Lock();
    }
    ~LockGuard()
    {
        _mutex.UnLock();
    }

private:
    Mutex _mutex;
};