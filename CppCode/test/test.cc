#include <coroutine>
#include <iostream>
#include <optional>

// 协程生成器，按需生成数列的协程对象
class Generator
{
public:
    struct promise_type;                                     // 协程生成器的核心，是协程的承诺类型，用于定义协程的行为
    using handle_type = std::coroutine_handle<promise_type>; // 管理协程的生命周期

    explicit Generator(handle_type h) : coro(h)
    {
    }
    ~Generator()
    {
        if (coro)
            coro.destroy();
    }
    // 函数恢复协程执行并返回协程是否完成
    bool next()
    {
        coro.resume();
        return not coro.done();
    }

    int value() const { return *coro.promise().current_value; }

public:
    struct promise_type // 协程生成器核心
    {
        std::optional<int> current_value; // 用于存储当前协程生成的值

        /// @brief 获取一个协程对象
        /// @return
        Generator get_return_object()
        {
            return Generator{handle_type::from_promise(*this)};
        }
        /// @brief 表示协程的初始挂起点，协程启动时是否挂起
        /// @return 需要挂起
        std::suspend_always initial_suspend()
        {
            return {};
        }
        /// @brief 协程结束时会挂起，等待释放资源
        /// @return 
        std::suspend_always final_suspend() noexcept
        {
            return {};
        }
        /// @brief 异常处理 终止程序
        void unhandled_exception() { std::exit(1); }
        /// @brief yield_value是协程的生成行为，调用co_yield时会调用这个函数，存到current_value中并返回
        /// @param value 
        /// @return 
        std::suspend_always yield_value(int value)
        {
            current_value = value;
            return {};
        }

        void return_void() {}
    };

private:
    handle_type coro;
};

Generator numberSequence(int start = 0, int step = 1)
{
    int value = start;
    while (true)
    {
        co_yield value;
        value += step;
    }
}

int main()
{
    auto seq = numberSequence(10, 5);
    for (int i = 0; i < 5; ++i)
    {
        seq.next();
        std::cout << seq.value() << " ";
    }
    return 0;
}
