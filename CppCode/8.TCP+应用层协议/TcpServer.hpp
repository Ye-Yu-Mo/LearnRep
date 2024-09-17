#pragma once

#include "Socket.hpp"
#include <iostream>
#include <pthread.h>
#include <functional>

using func_t = std::function<std::string(std::string &, bool *error_code)>;

class TcpServer;

class ThreadData
{
public:
    ThreadData(TcpServer *tcp_this, NetWork::Socket *sockp)
        : _this(tcp_this), _sockp(sockp)
    {
    }

public:
    TcpServer *_this;
    NetWork::Socket *_sockp;
};

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t handler_request)
        : _port(port), _listensocket(new NetWork::TcpSocket()), _handler_request(handler_request)
    {
    }
    static void *ThreadRun(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);

        std::string inbufferstream;
        while (true)
        {
            bool ok = true;
            // 读取报文
            if (!td->_sockp->Recv(&inbufferstream, 1024))
                ;
            break;
            // 处理报文
            std::string send_string = td->_this->_handler_request(inbufferstream, &ok);
            if (ok)
            {
                // 发送数据
                if (!send_string.empty())
                {
                    td->_sockp->Send(send_string);
                }
            }
            else
            {
                break;
            }
        }
        td->_sockp->CloseSocket();
        delete td->_sockp;
        delete td;
        return nullptr;
    }
    void Loop()
    {
        while (true)
        {
            std::string peerip;
            uint16_t peerport;
            NetWork::Socket *newsock = _listensocket->AcceptConnection(&peerip, &peerport);
            if (newsock == nullptr)
                continue;
            std::cout << "get a new connection,sockfd:" << newsock->GetSockFd() << "client info:" << peerip << ":" << peerport << std::endl;
            pthread_t tid;
            ThreadData *td = new ThreadData(this, newsock);
            pthread_create(&tid, nullptr, ThreadRun, td);
        }
    }
    ~TcpServer()
    {
        delete _listensocket;
    }

private:
    int _port;
    NetWork::Socket *_listensocket;

public:
    func_t _handler_request;
};