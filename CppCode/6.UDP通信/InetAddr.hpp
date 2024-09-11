#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class InetAddr // 将接收到的网络信息格式化
{
public:
    InetAddr(struct sockaddr_in &addr)
        : _addr(addr)
    {
        _port = ntohs(_addr.sin_port);   // 将网络字节序转换为主机字节序
        _ip = inet_ntoa(_addr.sin_addr); // 将网络字节序的IP转换为点分十进制的字符串
    }
    std::string Ip()
    {
        return _ip;
    }
    uint16_t Port()
    {
        return _port;
    }
    std::string PrintDebug() // 输出调试信息
    {
        std::string info = _ip;
        info += ':';
        info += std::to_string(_port);
        return info;
    }
    ~InetAddr() {}

private:
    std::string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;
};