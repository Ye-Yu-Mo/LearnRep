#include <coroutine>
#include <iostream>
using namespace std;

/// @brief 协程函数的返回值类型
struct CoRet
{
    /// @brief 用于控制协程的运行，协程内外数据的交换
    struct promise_type
    {
        /// @return 提供了两个可以被 co_await 的对象
        /// suspend_never 不暂停 继续运行
        /// suspend_always 暂停等待 执行逻辑跳回到调用的地方
        suspend_never initial_suspend()
        {
            cout << "initial_suspend" << endl;

            return {};
        }
        suspend_never final_suspend() noexcept
        {
            cout << "final_suspend" << endl;

            return {};
        }
        /// @brief 异常处理
        void unhandled_exception()
        {
        }
        /// @note 这个函数是用于获取协程暂停时返回的对象
        /// 这里其实可以返回的是一个任意对象
        /// 但是没有办法通过协程的返回值来控制协程的进行
        /// 最一般的方式是这样的 利用 promise 对象返回一个控制协程运行的句柄
        CoRet get_return_object()
        {
            cout << "get_return_object" << endl;

            return {coroutine_handle<promise_type>::from_promise(*this)};
        }
        suspend_always yield_value(int r)
        {
            out = r;
            return {};
        }
        int out;

        void return_value(int r)
        {
            res = r;
        }
        int res;
    };
    // 控制协程运行的句柄
    //  _h.resume() -> 继续运行协程, _h() -> 也是的
    coroutine_handle<promise_type> _h;
};

struct Note
{
    int num;
};

struct Input
{
    Note &in;
    /// @brief 当遇到 co_await 时 是否需要暂停当前协程并跳转回去
    /// @return 不需要暂停跳转返回true | 需要暂停等待返回false
    bool await_ready()
    {
        cout << "await_ready" << endl;
        return false;
    }

    /// @brief 定义在协程即将要被暂停时的一些行为
    /// @param h 当前协程的管理句柄
    /// @note 在即将暂停之间，我们可以得到管理句柄，并且执行一些操作
    /// @return 这个函数的返回值指的是 在这个协程在暂停之后，代码执行要跳转到的地方
    /// 如果是 void 则跳转到调用这个协程的地方
    /// 如果是 其他协程的句柄 则跳转到对应协程继续运行
    /// 类似于析构函数和构造函数
    /// 调用子类的构造函数时，也必定会自动调用父类的构造函数
    /// 这里就是 在遇到暂停之后，可以使用void返回到原来调用协程的地方，也可以继续调用其他协程
    /// 不同之处就是这里需要显式写出来
    void await_suspend(coroutine_handle<CoRet::promise_type> h)
    {
        cout << "await_suspend" << endl;
        // h.promise(); 这里协程管理句柄的promise成员函数返回的对象的作用
        // 其实就是获取协程的管理对象
    }

    /// @brief 在 co_wait 需要一个返回值的时候 才需要写
    /// @return 这里的返回值类型是可以自己设定的，可以代表不同的含义
    int await_resume()
    {
        cout << "await_resume" << endl;
        return in.num;
    }

};

/// @brief
/// @return 协程函数的返回值类型
/// @note
/// 虽然这个函数没有任何的返回语句，但是他仍然可以拥有返回类型
/// 这个返回类型就包含了这个协程的管理句柄，还有一些回调函数
CoRet Guess(Note &note)
{
    // 在这里编译器会隐含的创建一个对象 当然这里是简化过后的
    // CoRet::promise_type promise;
    // 利用对象产生一个返回的对象，每一次协程被暂停，返回的就是这个对象
    // CoRet ret = promise.get_return_object();
    // 用创建的对象 co_await initial_suspend 的结果
    // co_await promise.initial_suspend();
    int res = (rand() % 114) + 1;
    Input input{note};
    int g = co_await input; // 运行到这里时跳转出去
    // co_await 后面可以加的是一个对象 但是也需要加一点东西
    cout << "协程：你猜的是 " << g << endl;
    co_yield (res > g ? 1 : (res == g ? 0 : -1));
    co_return res;
    // co_await promise.final_suspend();
}

int main()
{
    srand(time(nullptr));
    Note note;
    auto ret = Guess(note); // 运行到 co_await 时跳转回来
    cout << "主函数：猜一猜" << endl;
    note.num = 10;
    ret._h.resume(); // 运行到这里时跳转回去
    cout << "主函数：猜的结果是：" << ((ret._h.promise().out == 1) ? "猜大了" : ((ret._h.promise().out == 0) ? "猜对了" : "猜小了"))
         << endl;
    cout << "主函数：随机数是：" << ret._h.promise().res << endl;
    return 0;
}