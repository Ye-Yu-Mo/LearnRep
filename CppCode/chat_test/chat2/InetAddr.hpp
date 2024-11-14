/**
 * @file InetAddr.hpp
 * @brief 网络地址类，用于格式化和管理接收到的网络信息。
 *
 * 该类负责将网络字节序的IP地址和端口号转换为可读的格式，并提供相应的访问方法。
 */
#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * @class InetAddr
 * @brief 处理网络地址的类
 *
 * 此类接收一个 `sockaddr_in` 类型的地址结构，并将其中的IP和端口信息转换为易于使用的格式。
 */
class InetAddr // 将接收到的网络信息格式化
{
public:
    /**
     * @brief InetAddr 构造函数
     * @param addr 一个引用类型的 `sockaddr_in` 结构体，包含网络地址信息。
     *
     * 构造函数中将网络字节序的端口号和IP地址转换为主机字节序和点分十进制字符串。
     */
    InetAddr(struct sockaddr_in &addr)
        : _addr(addr)
    {
        _port = ntohs(_addr.sin_port);   // 将网络字节序转换为主机字节序
        _ip = inet_ntoa(_addr.sin_addr); // 将网络字节序的IP转换为点分十进制的字符串
    }
    /**
     * @brief 获取IP地址
     * @return 返回点分十进制格式的IP地址字符串。
     */
    std::string Ip()
    {
        return _ip;
    }
    /**
     * @brief 获取端口号
     * @return 返回端口号（主机字节序）。
     */
    uint16_t Port()
    {
        return _port;
    }
    /**
     * @brief 输出调试信息
     * @return 返回格式化的调试信息字符串，包含IP地址和端口号。
     */
    std::string PrintDebug() // 输出调试信息
    {
        std::string info = _ip;
        info += ':';
        info += std::to_string(_port);
        return info;
    }
    ~InetAddr() {}

private:
    std::string _ip;          ///< 存储IP地址的字符串
    uint16_t _port;           ///< 存储端口号
    struct sockaddr_in _addr; ///< 存储网络地址信息的结构体
};