/**
 * @file threadpool.hpp
 * @brief 线程池类的实现
 *
 * 本文件实现了一个简单的线程池类，用于管理多线程任务的执行。
 */
#include <iostream>
#include <functional>
#include <memory>
#include <thread>
#include <future>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <atomic>
#include "logs/Xulog.h"

/**
 * @class threadpool
 * @brief 线程池类
 *
 * threadpool 类用于创建和管理线程池，以便在多线程环境中并发执行任务。
 */

class threadpool
{
public:
    using ptr = std::unique_ptr<threadpool>;   ///< 线程池管理句柄
    using Functor = std::function<void(void)>; ///< 线程池任务类型
    /**
     * @brief 构造函数
     * @param thr_count 线程数量，默认为 5
     *
     * 构造线程池并创建指定数量的工作线程。
     */
    threadpool(int thr_count = 5) : _stop(false)
    {
        for (int i = 0; i < thr_count; i++)
            _threads.emplace_back(&threadpool::entry, this);
    }
    /**
     * @brief 析构函数
     *
     * 停止线程池并等待所有线程完成。
     */
    ~threadpool()
    {
        stop();
    }
    /**
     * @brief 停止线程池
     *
     * 停止线程池并唤醒所有正在等待的线程，等待它们完成。
     */
    void stop()
    {
        if (_stop == true)
            return;
        _stop = true;
        _cv.notify_all(); // 唤醒线程
        for (auto &thread : _threads)
            thread.join();
    }

    /**
     * @brief 提交任务到线程池
     * @tparam F 可调用对象类型
     * @tparam Args 参数类型
     * @param func 要执行的函数
     * @param args 函数的参数
     * @return std::future<decltype(func(args...))> 返回任务的未来对象
     *
     * 将可调用对象封装成任务并提交到线程池中执行 自动推导返回值类型。
     */
    template <typename F, typename... Args>
    auto push(F &&func, Args &&...args) -> std::future<decltype(func(args...))>
    {
        // 将传入函数封装成packaged_task任务包
        using return_type = decltype(func(args...));
        auto tmp_func = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
        auto task = std::make_shared<std::packaged_task<return_type()>>(tmp_func);
        std::future<return_type> fu = task->get_future();
        // 构造lambda表达式(捕获任务对象,函数内执行任务对象)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            // 将构造出来的匿名对象传入任务池
            _taskpool.push_back([task]()
                                { (*task)(); });
            _cv.notify_one();
        }
        return fu;
    }

private:
    /**
     * @brief 线程入口函数
     *
     * 从任务池中取出任务并执行，直到线程池停止。
     */
    void entry()
    {
        while (!_stop)
        {
            // 临时任务池
            // 避免频繁加解锁
            std::vector<Functor> tmp_taskpool;
            {
                // 加锁
                std::unique_lock<std::mutex> lock(_mutex);
                // 等待任务不为空或_stop被置为1
                _cv.wait(lock, [this]()
                         { return _stop || !_taskpool.empty(); });

                // 取出任务进行执行
                tmp_taskpool.swap(_taskpool);
            }
            for (auto &task : tmp_taskpool)
            {
                task();
            }
        }
    }

private:
    std::atomic<bool> _stop;           ///< 标志位，指示线程池是否停止
    std::vector<Functor> _taskpool;    ///< 存放待执行任务的任务池
    std::mutex _mutex;                 ///< 互斥锁，用于保护任务池的访问
    std::condition_variable _cv;       ///< 条件变量，用于线程间同步
    std::vector<std::thread> _threads; ///< 存放工作线程的容器
};