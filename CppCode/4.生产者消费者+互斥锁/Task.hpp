#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

#define DEFAULT_VALUE 0

enum
{
    OK = 0,
    div_zero,
    mod_zero,
    unknow
};

const std::string operators = "+-*/%"; // 加减乘除和模拟其他未知操作

class Task
{
public:
    Task()
    {
    }
    Task(int x, int y, char op)
        : data_x(x), data_y(y), Operator(op), result(DEFAULT_VALUE), code(OK)
    {
    }

    void Run()
    {
        switch (Operator)
        {
        case '+':
            result = data_x + data_y;
            break;
        case '-':
            result = data_x - data_y;
            break;
        case '*':
            result = data_x * data_y;
            break;
        case '/':
            if (data_y == 0)
                code = div_zero;
            else
                result = data_x / data_y;
            break;
        case '%':
            if (data_y == 0)
                code = mod_zero;
            else
                result = data_x % data_y;
            break;
        default:
            code = unknow;
            break;
        }
    }

    void operator()()
    {
        Run();
        sleep(2);
    }

    std::string PrintTask()
    {
        return std::to_string(data_x) + Operator + std::to_string(data_y) + "=?";
    }

    std::string PrintResult()
    {
        return std::to_string(data_x) + Operator + std::to_string(data_y) + "=" + std::to_string(result) + "[" + std::to_string(code) + "]";
    }
    ~Task()
    {
    }

private:
    int data_x;
    int data_y;
    char Operator;

    int result;
    int code; // 任务完成的怎么样，是否有错误
};