#include "Socket.hpp"
#include "Threadpool.hpp"
#include <iostream>
#include <functional>

using CallBack = std::function<std::string(std::string &, bool *error_code)>;

class TcpServer;
class ThreadData
{
public:
    ThreadData(TcpServer *tcp_this, NetWork::TcpSocket *sockp)
        : _this(tcp_this), _sockp(sockp) {}

public:
    TcpServer *_this;
    NetWork::TcpSocket *_sockp;
};

class TcpServer
{
public:
    TcpServer(uint16_t port, CallBack call_back, int thread_count = 5)
        : _port(port), _listen_socket(new NetWork::TcpSocket()),
          _call_back(call_back), _thread_pool(std::make_unique<threadpool>(thread_count))
    {
        std::cout << "TCP服务器已经创建！" << std::endl;
        _listen_socket->BuildListenSocketMethod(port);
    }

    static void *ThreadRun(void *args)
    {
        std::string in_buf_stream;
        ThreadData *td = static_cast<ThreadData *>(args);
        while (true)
        {
            bool ok = true;
            if (!td->_sockp->Recv(&in_buf_stream))
                break;
            std::string send_string = td->_this->_call_back(in_buf_stream, &ok);
            if (ok)
                if (!send_string.empty())
                    td->_sockp->Send(send_string);
                else
                    break;
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
            std::string peer_ip;
            uint16_t peer_port;
            NetWork::TcpSocket *newsock = _listen_socket->AcceptConnection(&peer_ip, &peer_port);
            if (newsock == nullptr)
                continue;
            std::cout << "get a new connection,sockfd:" << newsock->GetSockFd() << "client info:" << peer_ip << ":" << peer_port << std::endl;
            ThreadData *td = new ThreadData(this, newsock);
            _thread_pool->push(ThreadRun, td);
        }
    }
    ~TcpServer() { delete _listen_socket; }

private:
    int _port;
    NetWork::TcpSocket *_listen_socket;
    std::unique_ptr<threadpool> _thread_pool;

public:
    CallBack _call_back;
};

std::string callBack(std::string &msg, bool *error_code)
{
    std::cout << "I get a msg : " + msg << std::endl;
    msg.clear();
    return "I get a msg : " + msg;
}

int main()
{
    std::unique_ptr<TcpServer> tps = std::make_unique<TcpServer>(8888, callBack);
    tps->Loop();
    return 0;
}