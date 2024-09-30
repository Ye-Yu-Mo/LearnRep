#include "protobuf/codec.h"
#include "protobuf/dispatcher.h"

#include "muduo/include/muduo/base/Logging.h"
#include "muduo/include/muduo/base/Mutex.h"
#include "muduo/include/muduo/net/EventLoop.h"
#include "muduo/include/muduo/net/TcpServer.h"

#include "Request.pb.h"
#include "TransLate.hpp"
#include "../logs/Xulog.h"

#include <iostream>
#include <unordered_map>
#include <unistd.h>

class Server
{
public:
    using MessagePtr = std::shared_ptr<google::protobuf::Message>;
    using TranslateRequestPtr = std::shared_ptr<Xu::TranslateRequest>;
    using TranslateResponsePtr = std::shared_ptr<Xu::TranslateResponse>;
    using AddRequestPtr = std::shared_ptr<Xu::AddRequest>;
    using AddResponsePtr = std::shared_ptr<Xu::AddResponse>;

    Server(int port)
        : _server(&_baseloop, muduo::net::InetAddress("0.0.0.0", port),
                  "Server", muduo::net::TcpServer::kReusePort),
          _dispatcher(std::bind(&Server::onUnknowMessage, this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
          _codec(std::bind(&ProtobufDispatcher::onProtobufMessage, &_dispatcher,
                           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
    {
        // 注册业务请求处理函数
        _dispatcher.registerMessageCallback<Xu::TranslateRequest>(std::bind(&Server::onTranslate, this, std::placeholders::_1,
                                                                            std::placeholders::_2, std::placeholders::_3));
        _dispatcher.registerMessageCallback<Xu::AddRequest>(std::bind(&Server::onAdd, this, std::placeholders::_1,
                                                                      std::placeholders::_2, std::placeholders::_3));
        _server.setMessageCallback(std::bind(&ProtobufCodec::onMessage, &_codec, std::placeholders::_1,
                                             std::placeholders::_2, std::placeholders::_3));
        _server.setConnectionCallback(std::bind(&Server::onConnection, this, std::placeholders::_1));
    }
    void start()
    {
        _server.start();
        _baseloop.loop();
    }

private:
    std::string translate(const std::string &str)
    {
        if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'z'))
            return Translate(str, "en", "zh");
        return Translate(str, "zh", "en");
    }

    void onAdd(const muduo::net::TcpConnectionPtr &conn, const AddRequestPtr message, muduo::Timestamp)
    {
        // 提取Message中的有效消息
        int num1 = message->num1();
        int num2 = message->num2();
        // 进行计算得到结果
        int ans = num1 + num2;
        // 组织并发送protobuf的相应
        Xu::AddResponse resp;
        resp.set_result(ans);
        _codec.send(conn, resp);
    }
    void onTranslate(const muduo::net::TcpConnectionPtr &conn, const TranslateRequestPtr message, muduo::Timestamp)
    {
        // 提取Message中的有效消息
        std::string req_msg = message->msg();
        // 进行翻译得到结果
        std::string rsp_msg = translate(req_msg);
        // 组织并发送protobuf的相应
        Xu::TranslateResponse resp;
        resp.set_msg(rsp_msg);
        _codec.send(conn, resp);
    }
    void onUnknowMessage(const muduo::net::TcpConnectionPtr &conn, const Server::MessagePtr message, muduo::Timestamp)
    {
        INFO("onUnknowMessage: %s", message->GetTypeName());
        conn->shutdown();
    }
    void onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())

            INFO("连接建立成功!");
        else
            INFO("连接关闭!");
    }

private:
    muduo::net::EventLoop _baseloop;
    muduo::net::TcpServer _server;  // 服务器对象
    ProtobufDispatcher _dispatcher; // 请求分发器对象 -> 注册请求处理函数
    ProtobufCodec _codec;           // protobuf协议处理器 -> 对收到的请求数据进行protobuf协议处理
};

int main()
{
    Server server(8085);
    server.start();
    return 0;
}