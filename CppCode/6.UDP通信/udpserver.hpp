#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "nocopy.hpp"
#include "InetAddr.hpp"

const static uint16_t defaultport = 1141; // 默认端口号
const static uint16_t defaultfd = -1;     // 默认socket
const static uint16_t defaultsize = 1024; // 默认缓冲区大小

class UdpServer : public nocopy // 服务器不允许被拷贝，简化的单例模式
{
public:
    UdpServer(uint16_t port = 11451)
        : _port(port)
    {
    }
    void Init()
    {
        // 创建socket文件描述符，对其进行初始化
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 使用IPv4，UDP协议
        if (_sockfd < 0)
        {
            perror("socket创建失败");
            exit(errno);
        }
        std::cout << "socket创建成功,文件描述符为:" << _sockfd << std::endl;

        // 初始化网络信息
        struct sockaddr_in local;
        bzero(&local, sizeof(local));       // memset
        local.sin_family = AF_INET;         // 协议簇
        local.sin_port = htons(_port);      // 端口号
        local.sin_addr.s_addr = INADDR_ANY; // ip地址

        // 绑定到系统内核
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n != 0)
        {
            perror("bind出错");
            exit(errno);
        }
    }
    void Start() // 服务器不退出
    {
        char buffer[1024];
        for (;;)
        {
            struct sockaddr_in peer; // 远程地址信息，因为需要返回请求
            socklen_t len = sizeof(peer);
            ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len); // 获取从从外部接收的数据
            // 第一个参数是套接字文件描述符，标识接收数据的套接字
            // 第二个参数是接收数据的缓冲区

            if (n > 0) // 正确接收
            {
                InetAddr addr(peer); // 格式化网络信息
                buffer[n] = '\0';    // 手动添加结束
                std::cout << '[' << addr.PrintDebug() << "]# " << buffer << std::endl;
                sendto(_sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&peer, len);
            }
        }
    }
    ~UdpServer()
    {
    }

private:
    uint16_t _port; // 端口号
    int _sockfd;    // socket文件描述符
};