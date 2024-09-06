#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#define LOG1(fmt, ...) \
    printf("[%s:%d]-" fmt, __FILE__, __LINE__, ##__VA_ARGS__);

void LOG2(int cnt, ...)
{
    va_list ap;
    va_start(ap, cnt); // 获取第一个参数的起始地址
    for (int i = 0; i < cnt; i++)
    {
        int num = va_arg(ap, int); // 获取之后的参数，需要指定类型
        std::cout << num << ' ';
    }
    std::cout << std::endl;
    va_end(ap); // ap指针置空
}

void LOG3(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *res;
    int ret = vasprintf(&res, fmt, ap);
    if (ret != -1)
        printf("%s", res);
    free(res);
    va_end(ap);
}

void LOG4()
{
    std::cout << std::endl;
}

template <typename T, typename... Args>
void LOG4(const T &v, Args &&...args)
{
    std::cout << v;
    if (sizeof ... (args))
    {
        LOG4(std::forward<Args>(args)...);
    }
}

int main()
{
    // LOG1("日志样例");
    // LOG2(2, 1, 2);
    // LOG3("[%s:%d]-%s", __FILE__, __LINE__, "日志样例");
    LOG4("日志样例1", "日志样例2", "日志样例3");
    return 0;
}