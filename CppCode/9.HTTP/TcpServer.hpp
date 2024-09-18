#pragma once

#include "Socket.hpp"
#include <iostream>
#include <pthread.h>
#include <functional>

using func_t = std::function<std::string(std::string &request)>;

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
        : _port(port), _handler_request(handler_request), _listensocket(new NetWork::TcpSocket())
    {
        _listensocket->BuildListenSocketMethod(_port, NetWork::backlog);
    }
    static void *ThreadRun(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        std::string http_request;
        if (td->_sockp->Recv(&http_request, 4096))
        {
            // MARK
            std::cout << http_request << std::endl;
            std::string http_response = td->_this->_handler_request(http_request);
            if (!http_response.empty())
            {
                td->_sockp->Send(http_response);
            }
        }

        td->_sockp->CloseSockFd();
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
            std::cout << "获取一个新连接,sockfd: " << newsock->GetSockFd() << " client info: " << peerip << ":" << peerport << std::endl;
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