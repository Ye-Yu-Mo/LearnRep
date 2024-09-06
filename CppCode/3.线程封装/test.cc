#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include "Thread.hpp"

int ticket = 10000;

std::string ThreadName()
{
    static int number = 1;
    char name[64];
    snprintf(name, sizeof(name), "Thread[%d]", number);
    return name;
}

void BuyTicket(int mutex)
{
    while (true)
    {
        if (ticket > 0)
        {
            usleep(1000); // 假设访问花费的时间
            ticket--;
            printf("剩余票数:%d\n", ticket);
        }
        else
        {
            break;
        }
    }
}

int main()
{
    std::vector<Thread<int>> Ts;
    for (int i = 0; i < 5; i++)
    {
        // Thread(const std::string &tname, Func_t<T> func, T data)
        std::string name = ThreadName();
        Thread<int> tmp(name, BuyTicket, 0);
        Ts.push_back(tmp);
    }

    for (int i = 0; i < 5; i++)
    {
        Ts[i].Start();
    }
    for (int i = 0; i < 5; i++)
    {
        Ts[i].Join();
    }
    return 0;
}