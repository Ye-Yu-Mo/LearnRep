#include "protobuf/dispatcher.h"
#include "protobuf/codec.h"

#include "muduo/include/muduo/base/Mutex.h"
#include "muduo/include/muduo/base/Logging.h"
#include "muduo/include/muduo/net/EventLoop.h"
#include "muduo/include/muduo/net/TcpClient.h"
#include "muduo/include/muduo/net/EventLoopThread.h"
#include "muduo/include/muduo/base/CountDownLatch.h"

#include "Request.pb.h"
#include "../logs/Xulog.h"

#include <iostream>
#include <unistd.h>

class Client
{
public:
    using MessagePtr = std::shared_ptr<google::protobuf::Message>;
    using TranslateResponsePtr = std::shared_ptr<Xu::TranslateResponse>;
    using AddResponsePtr = std::shared_ptr<Xu::AddResponse>;
    Client(const std::string &sip, int sport)
        : _latch(1), _client(_loopthread.startLoop(), muduo::net::InetAddress(sip, sport), "Client"),
          _dispatcher(std::bind(&Client::onUnknowMessage, this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
          _codec(std::bind(&ProtobufDispatcher::onProtobufMessage, &_dispatcher,
                           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
    {
        _dispatcher.registerMessageCallback<Xu::TranslateResponse>(std::bind(&Client::onTranslate, this, std::placeholders::_1,
                                                                             std::placeholders::_2, std::placeholders::_3));
        _dispatcher.registerMessageCallback<Xu::AddResponse>(std::bind(&Client::onAdd, this, std::placeholders::_1,
                                                                       std::placeholders::_2, std::placeholders::_3));
        _client.setMessageCallback(std::bind(&ProtobufCodec::onMessage, &_codec, std::placeholders::_1,
                                             std::placeholders::_2, std::placeholders::_3));
        _client.setConnectionCallback(std::bind(&Client::onConnection, this, std::placeholders::_1));
    }
    // 连接服务器 阻塞等待连接建立成功
    void connect()
    {
        _client.connect();
        _latch.wait(); // 阻塞等待,直到连接建立成功
    }
    void Translate(const std::string &str)
    {
        Xu::TranslateRequest req;
        req.set_msg(str);

        send(&req);
    }
    void Add(int num1, int num2)
    {
        Xu::AddRequest req;
        req.set_num1(num1);
        req.set_num2(num2);

        send(&req);
    }

private:
    bool send(const google::protobuf::Message *msg)
    {
        if (_conn->connected())
        {
            _codec.send(_conn, *msg);
            return true;
        }
        return false;
    }

    void onAdd(const muduo::net::TcpConnectionPtr &conn, const AddResponsePtr message, muduo::Timestamp)
    {
        INFO("加法结果是: %d", message->result());
    }
    void onTranslate(const muduo::net::TcpConnectionPtr &conn, const TranslateResponsePtr message, muduo::Timestamp)
    {
        INFO("翻译结果是: %s", message->msg().c_str());
    }
    void onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            _latch.countDown();
            _conn = conn;
        }
        else
        {
            _conn.reset();
        }
    }

    void onUnknowMessage(const muduo::net::TcpConnectionPtr &conn, const Client::MessagePtr message, muduo::Timestamp)
    {
        INFO("onUnknowMessage: %s", message->GetTypeName());
        conn->shutdown();
    }

private:
    muduo::CountDownLatch _latch;            // 实现同步
    muduo::net::EventLoopThread _loopthread; // 异步循环处理线程
    muduo::net::TcpConnectionPtr _conn;      // 客户端连接
    muduo::net::TcpClient _client;           // 客户端句柄
    ProtobufDispatcher _dispatcher;          // 请求分发器
    ProtobufCodec _codec;                    // 协议处理器
};

int main()
{
    Client client("127.0.0.1", 8085);
    client.connect();
    client.Translate("Hello");
    client.Add(1, 6);
    sleep(1);
    return 0;
}