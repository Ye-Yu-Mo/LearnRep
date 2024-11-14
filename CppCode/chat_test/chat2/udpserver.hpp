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
#include "logs/Xulog.h"

const static uint16_t PORT = 12346;      ///< 默认端口号
const static uint16_t DEFAULTFD = -1;    ///< 默认socket
const static uint16_t BUFFERSIZE = 1024; ///< 默认缓冲区大小
/**
 * @brief UdpServer类用于实现UDP服务器功能
 *
 * 该类提供了UDP服务器的基本操作，包括初始化socket和接收数据。
 * 该服务器不支持复制，因此继承自nocopy类。
 */
class UdpServer : public nocopy // 服务器不允许被拷贝，简化的单例模式
{
public:
    using ptr = std::unique_ptr<UdpServer>; ///< 服务器管理句柄
    /**
     * @brief 构造函数
     *
     * @param port 服务器监听的端口号，默认为12345
     */
    UdpServer(uint16_t port = PORT)
        : _port(port)
    {
    }
    /**
     * @brief 初始化服务器
     *
     * 创建socket文件描述符并绑定到指定的端口上。
     * 如果socket创建或绑定失败，将输出错误日志并退出程序。
     */
    void Init()
    {
        // 创建socket文件描述符，对其进行初始化
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 使用IPv4，UDP协议
        if (_sockfd < 0)
        {
            FATAL("socket创建失败");
            exit(errno);
        }
        INFO("socket创建成功,sockfd:%d", _sockfd);

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
            FATAL("bind出错");
            exit(errno);
        }
    }
    /**
     * @brief 启动服务器
     *
     * 进入无限循环，接收外部发送的数据，并打印远程地址及内容。
     * 服务器在接收数据时不会退出。
     */
    void Start() // 服务器不退出
    {
        DEBUG("服务端已经启动 正在接收消息");
        char buffer[BUFFERSIZE];
        while (true)
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
                std::cout << "Please Enter# ";
            }
        }
    }
    /**
     * @brief 析构函数
     *
     * 释放UdpServer类的资源。
     */
    ~UdpServer()
    {
        close(_sockfd);
    }

private:
    uint16_t _port; // 端口号
    int _sockfd;    // socket文件描述符
};