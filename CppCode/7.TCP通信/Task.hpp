#pragma once
#include "InetAddr.hpp"

class Task
{
public:
    void operator()()
    {
    }

private:
    int sockfd;
    InetAddr addr;
};