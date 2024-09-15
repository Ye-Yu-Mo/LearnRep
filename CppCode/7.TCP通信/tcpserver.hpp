#pragma once

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <unordered_map>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "nocopy.hpp"
#include "comm.hpp"
#include "InetAddr.hpp"
#include "ThreadPool.hpp"

const int default_backlog = 5;
using task_t = std::function<void()>;
using callback_t = std::function<void(int, InetAddr &)>;

class TcpServer : public nocopy
{
public:
    TcpServer(uint16_t port)
        : _port(port), _isrunning(false)
    {
    }
    void Init()
    {
        // 创建socket,本质就是文件描述符
        _listensock = socket(AF_INET, SOCK_STREAM, 0); // IPv4 TCP
        if (_listensock < 0)
        {
            std::cout << "socket创建失败,错误码为" << errno << "错误信息为" << strerror(errno) << std::endl;
            exit(1);
        }
        std::cout << "socket创建成功,sockfd为" << _listensock << std::endl;

        // 填充本地网络信息并绑定
        int opt = 1;
        setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(_listensock, CONV(&local), sizeof(local)) != 0)
        {
            std::cout << "绑定失败\n";
            exit(1);
        }
        std::cout << "socket绑定成功,sockfd为" << _listensock << std::endl;

        // 设置socket为监听状态
        if (listen(_listensock, default_backlog) != 0)
        {
            std::cout << "监听失败\n";
            exit(1);
        }
        std::cout << "socket监听成功,sockfd为" << _listensock << std::endl;

        ThreadNs::ThreadPool<task_t>::GetInstance()->Start();
        funcs.insert(std::make_pair("defaultService", std::bind(&TcpServer::DefaultService, this, std::placeholders::_1, std::placeholders::_2)));
    }
    void Service(int sockfd, InetAddr addr)
    {
        char buffer[1024];

        while (true)
        {
            ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                buffer[n] = 0;
                std::cout << addr.PrintDebug() << "# " << buffer << std::endl;
                std::string echo_string = "server echo# ";
                echo_string += buffer;
                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if (n == 0)
            {
                std::cout << "客户机退出\n";
                break;
            }
            else
            {
                std::cout << "socker读取错误\n";
            }
        }
    }
    void Start()
    {
        _isrunning = true;
        signal(SIGCHLD, SIG_IGN);
        while (_isrunning)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sockfd = accept(_listensock, CONV(&peer), &len);
            if (sockfd < 0)
            {
                std::cout << "socket接受失败\n";
                continue;
            }
            std::cout << "socket接受成功,获得新sockfd:" << sockfd << std::endl;
            task_t t = std::bind(&TcpServer::Routinue, this, sockfd, InetAddr(peer));
            ThreadNs::ThreadPool<task_t>::GetInstance()->Push(t);
        }
    }
    std::string Read(int sockfd)
    {
        char type[1024];
        ssize_t n = read(sockfd, type, sizeof(type) - 1);
        if (n > 0)
        {
            type[n] = '\0';
        }
        else if (n == 0)
        {
            std::cout << "客户机退出\n";
        }
        else
        {
            std::cout << "socket读取失败\n";
        }
        return type;
    }
    void Routinue(int sockfd, InetAddr addr)
    {
        funcs["defaultService"](sockfd, addr);
        std::string type = Read(sockfd);
        std::cout << addr.PrintDebug().c_str() << "选择了" << type.c_str() << std::endl;
        if (type == "ping" || type == "translate" || type == "transform")
            funcs[type](sockfd, addr);
        close(sockfd);
    }
    void DefaultService(int sockfd, InetAddr &addr)
    {
        (void)addr;
        std::string service_list = "|";
        for (auto func : funcs)
        {
            service_list += func.first;
            service_list += "|";
        }
        write(sockfd, service_list.c_str(), service_list.size());
    }
    void RegisterFunc(const std::string &name, callback_t func)
    {
        funcs[name] = func;
    }
    ~TcpServer()
    {
    }

private:
    uint16_t _port;
    int _listensock;
    bool _isrunning;

    // 业务处理逻辑
    std::unordered_map<std::string, callback_t> funcs;
};