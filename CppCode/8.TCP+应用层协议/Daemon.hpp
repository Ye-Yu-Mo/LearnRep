#pragma once

#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

const char *root = "/";             // 根目录
const char *dev_null = "/dev/null"; // 销毁

void Daemon(bool ischdir, bool isclose)
{
    // 忽略会引起进程异常退出的信号
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    // 自己不能是组长
    if (fork() > 0) // 创建一个子进程，如果自己是父进程则退出，子进程则继续走下去
        exit(0);

    // 创建新的会话，从此都是子进程
    setsid();

    // 每个进程都有一个CWD，当前工作目录，更改为根目录
    if (ischdir)
        chdir(root);

    // 关闭标准输入、输出、错误
    if (isclose)
    {
        close(0);
        close(1);
        close(2);
    }
    else
    {
        int fd = open(dev_null, O_RDWR);
        if (fd > 0)
        {
            dup2(fd, 0);
            dup2(fd, 1);
            dup2(fd, 2);
            close(fd);
        }
    }
}