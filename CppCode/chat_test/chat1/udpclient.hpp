/**
 * @file udp_client.hpp
 * @brief UDP 客户端类的定义
 *
 * 该文件定义了一个 UDP 客户端类，用于与指定的服务器进行数据通信。
 * 它使用 C++ 标准库中的智能指针管理资源，并使用原始套接字 API 进行网络操作。
 *
 */
#include <iostream>
#include <cerrno>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "nocopy.hpp"
#include "logs/Xulog.h"


/**
 * @class UdpClient
 * @brief UDP 客户端类
 *
 * 该类用于创建一个 UDP 客户端，可以向指定的服务器发送数据。
 * 它不允许拷贝实例（通过继承 nocopy 类实现）。
 */
class UdpClient : public nocopy
{
public:
    using ptr = std::unique_ptr<UdpClient>; ///< 客户端管理句柄
    /**
     * @brief 构造函数
     * @param serverip 服务器的 IP 地址
     *
     * 构造函数创建一个 UDP socket，并初始化服务器信息。
     *
     * @note 客户端在第一次发送数据时会自动绑定本地地址。
     */
    UdpClient(const std::string &serverip)
    {
        uint16_t serverport = 12346;
        // 创建socket
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            FATAL("socket创建失败");
            exit(errno);
        }
        INFO("socket创建成功,sockfd:%d", _sockfd);
        // 客户机也需要绑定，但是不需要主动调用，客户机会在第一次发送数据时自动绑定数据
        // 填充服务器信息
        memset(&_server_info, 0, sizeof(_server_info));
        _server_info.sin_family = AF_INET;
        _server_info.sin_port = htons(serverport);
        _server_info.sin_addr.s_addr = inet_addr(serverip.c_str());
    }
    /**
     * @brief 启动客户端
     *
     * 该函数启动客户端，并不断接收用户输入以发送消息到服务器。
     */
    void Start()
    {
        DEBUG("客户端已经启动 正则等待发送消息");
        while (true)
        {
            // 发送的数据
            std::string inbuffer;
            std::cout << "Please Enter# ";
            std::getline(std::cin, inbuffer);
            // 发送消息（请求）
            ssize_t n = sendto(_sockfd, inbuffer.c_str(), inbuffer.size(), 0, (struct sockaddr *)&_server_info, sizeof(_server_info));
        }
    }
    /**
     * @brief 析构函数
     *
     * 该函数关闭 socket 描述符。
     */
    ~UdpClient()
    {
        close(_sockfd);
    }

private:
    struct sockaddr_in _server_info; ///< 服务器信息结构体
    int _sockfd;                     ///< socket 描述符
};
