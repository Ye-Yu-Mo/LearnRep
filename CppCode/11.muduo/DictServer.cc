#include "muduo/include/muduo/net/TcpServer.h"
#include "muduo/include/muduo/net/EventLoop.h"
#include "muduo/include/muduo/net/TcpConnection.h"
#include "../logs/Xulog.h"
#include "TransLate.hpp"
#include <iostream>
#include <functional>
#include <unordered_map>

class TranslateServer
{
public:
    TranslateServer(int port) : _server(&_baseloop,
                                        muduo::net::InetAddress("0.0.0.0", port),
                                        "TranslateServer",
                                        muduo::net::TcpServer::kReusePort)
    {
        // 参数绑定
        auto func_1 = std::bind(&TranslateServer::onConnection, this, std::placeholders::_1);
        auto func_2 = std::bind(&TranslateServer::onMessage, this, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3);
        // 设置回调函数
        _server.setConnectionCallback(func_1);
        _server.setMessageCallback(func_2);
    }

    // 启动服务器
    void start()
    {
        _server.start();  // 开始事件监听
        _baseloop.loop(); // 开始事件监控,死循环阻塞接口
    }

private:
    // 建立连接或关闭之后的回调函数
    void onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            INFO("新连接建立成功!");
        }
        else
        {
            INFO("连接关闭!");
        }
    }
    std::string translate(const std::string &str)
    {
        if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'z'))
            return Translate(str, "en", "zh");
        return Translate(str, "zh", "en");
    }
    // 收到请求时的回调函数
    void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp)
    {
        // 调用translate接口进行翻译,向客户端返回结果
        std::string str = buf->retrieveAllAsString();
        std::string resp = translate(str);
        conn->send(resp);
    }

private:
    muduo::net::EventLoop _baseloop;
    muduo::net::TcpServer _server;
};

int main()
{
    TranslateServer server(8085);
    server.start();
    return 0;
}