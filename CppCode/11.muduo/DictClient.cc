#include "muduo/include/muduo/net/TcpClient.h"
#include "muduo/include/muduo/net/EventLoopThread.h"
#include "muduo/include/muduo/net/TcpConnection.h"
#include "muduo/include/muduo/base/CountDownLatch.h"
#include "../logs/Xulog.h"
#include <iostream>
#include <functional>

bool flag = true;

class TranslateClient
{
public:
    TranslateClient(const std::string &sip, int sport) : _lanch(1), // 设置阻塞
                                                         _client(_loopthread.startLoop(), muduo::net::InetAddress(sip, sport), "TranslateClient")

    {
        auto func_1 = std::bind(&TranslateClient::onConnection, this, std::placeholders::_1);
        auto func_2 = std::bind(&TranslateClient::onMessage, this, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3);
        _client.setConnectionCallback(func_1);
        _client.setMessageCallback(func_2);
    }
    // 连接服务器 阻塞等待连接建立成功
    void connect()
    {
        _client.connect();
        _lanch.wait(); // 阻塞等待,直到连接建立成功
    }
    bool send(const std::string &msg)
    {
        if (_conn->connected())
        {
            _conn->send(msg);
            return true;
        }
        return false;
    }

private:
    // 建立连接或关闭之后的回调函数 唤醒阻塞
    void onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            _lanch.countDown(); // 唤醒主线程阻塞
            _conn = conn;
            std::cout << "连接建立成功!" << std::endl;
        }
        else
        {
            _conn.reset();
            std::cout << "连接已经断开!" << std::endl;
        }
    }

    // 收到消息时的回调函数
    void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp)
    {
        std::cout << "翻译完成啦,结果是: " << buf->retrieveAllAsString() << std::endl;
        flag = true;
    }

private:
    muduo::CountDownLatch _lanch;
    muduo::net::EventLoopThread _loopthread;
    muduo::net::TcpClient _client;
    muduo::net::TcpConnectionPtr _conn;
};

int main()
{
    TranslateClient client("127.0.0.1", 8085);
    client.connect();
    std::cout << "按下ctrl+c退出程序\n";
    while (true)
    {
        while (flag == false);

        std::cout << "请输入你要翻译的内容:>" << std::flush;
        flag = false;

        std::string buffer;
        std::getline(std::cin, buffer);
        client.send(buffer);
    }
    return 0;
}