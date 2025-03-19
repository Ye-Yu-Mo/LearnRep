#include <coroutine>
#include <iostream>
#include <cstdint>

struct task {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    
    handle_type coro_handle;
    explicit task(handle_type h) : coro_handle(h) {}
    
    struct promise_type {
        task get_return_object() { 
            return task{handle_type::from_promise(*this)}; 
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

class Scheduler {
    std::coroutine_handle<> coros[3];
    int current = 0;
    uint32_t counter = 1;
public:
    void set_coroutine(int idx, std::coroutine_handle<> h) { coros[idx] = h; }

    bool schedule() {
        if (counter > 100) return false;
        
        coros[current].resume();
        current = (current + 1) % 3;
        return true;
    }

    void increment() { ++counter; }
    uint32_t get_counter() const { return counter; }
};

Scheduler scheduler;

task coroutine_func(int id) {
    while (scheduler.get_counter() <= 100) {
        if (scheduler.get_counter() <= 100) {
            std::cout << "Coroutine " << id 
                      << ": " << scheduler.get_counter() 
                      << std::endl;
            scheduler.increment();
        }
        co_await std::suspend_always{};
    }
}

int main() {
    auto h1 = coroutine_func(1).coro_handle;
    auto h2 = coroutine_func(2).coro_handle; 
    auto h3 = coroutine_func(3).coro_handle;
    
    scheduler.set_coroutine(0, h1);
    scheduler.set_coroutine(1, h2);
    scheduler.set_coroutine(2, h3);

    while (scheduler.schedule()) {}
    
    h1.destroy();
    h2.destroy();
    h3.destroy();
}
