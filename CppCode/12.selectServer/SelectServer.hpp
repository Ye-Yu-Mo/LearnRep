#include "../logs/Xulog.h"
#include "Socket.hpp"
#include <iostream>
#include <string>
#include <sys/select.h>
#include <memory>

using namespace NetWork;

const static int GDefaultPort = 8888;
const static int Gbacklog = 8;
const static int FD_MAX_NUM = sizeof(fd_set) * 8;

class SelectServer
{
private:
    void HandlerEvent(fd_set &rfds)
    {
        for (int i = 0; i < FD_MAX_NUM; i++)
        {
            if (_rfds_array[i] == nullptr)
                continue;
            int fd = _rfds_array[i]->GetSockFd();
            if (FD_ISSET(fd, &rfds))
            {
                if (fd == _listensock->GetSockFd())
                {
                    INFO("Get a new link!");
                    std::string clientip;
                    uint16_t clientport;
                    Socket *sock = _listensock->AcceptConnection(&clientip, &clientport);
                    if (!sock)
                    {
                        ERROR("Accept ERROR!");
                        continue;
                    }
                    INFO("Get a client, client info is# %s:%d, fd:%d", clientip.c_str(), clientip, sock->GetSockFd());
                    int pos = 0;
                    for (; pos < FD_MAX_NUM; pos++)
                    {
                        if (_rfds_array[pos] == nullptr)
                        {
                            _rfds_array[pos] = sock;
                            INFO("Get A new Link, fd is : %d", sock->GetSockFd());
                            break;
                        }
                    }
                    if (pos == FD_MAX_NUM)
                    {
                        sock->CloseSockFd();
                        delete sock;
                        WARN("Server is Full!");
                    }
                }
                else
                {
                    std::string buffer;
                    bool res = _rfds_array[i]->Recv(&buffer, 1024);
                    if (res)
                    {
                        INFO("Client say# %s", buffer.c_str());
                        buffer += "Hi, I See YOU";
                        _rfds_array[i]->Send(buffer);
                        buffer.clear();
                    }
                    else
                    {
                        WARN("Client Quit, Close or Error, close fd: %d", _rfds_array[i]->GetSockFd());
                        _rfds_array[i]->CloseSockFd();
                        delete _rfds_array[i];
                        _rfds_array[i] = nullptr;
                    }
                }
            }
        }
    }

public:
    SelectServer(int port = GDefaultPort)
        : _port(port), _listensock(new TcpSocket()), _isrunning(false)
    {
    }
    void InitServer()
    {
        _listensock->BuildListenSocketMethod(_port, Gbacklog);
        for(int i=0; i<FD_MAX_NUM;i++)
        {
            _rfds_array[i] = nullptr;
        }
        _rfds_array[0] = _listensock.get();
    }
    void Loop()
    {
        _isrunning = true;
        while (_isrunning)
        {
            fd_set rfds; // read fd set
            FD_ZERO(&rfds);
            int max_fd = _listensock->GetSockFd(); // 获得listen的文件描述符
            for (int i = 0; i < FD_MAX_NUM; i++)
            {
                if (_rfds_array[i] == nullptr)
                {
                    continue;
                }
                else
                {
                    int fd = _rfds_array[i]->GetSockFd();
                    FD_SET(fd, &rfds); // 插入到fd_set中
                    if (max_fd < fd)
                    {
                        max_fd = fd; // 找到最大的fd
                    }
                }
            }

            struct timeval timeout = {0, 0}; // 不阻塞等待
            PrintDebug();
            int n = select(max_fd + 1, &rfds, nullptr, nullptr, nullptr);
            switch (n)
            {
            case 0:
                INFO("Select timeout, last time: %u.%u\n", timeout.tv_sec, timeout.tv_usec);
                break;
            case -1:
                ERROR("Select ERROR");
                break;
            default:
                INFO("Select Success, Begin event handler, last time: %u.%u", timeout.tv_sec, timeout.tv_usec);
                HandlerEvent(rfds);
                break;
            }
            _isrunning = false;
        }
    }
    void PrintDebug()
    {
        std::cout << "current select read fd set is : ";
        for (int i = 0; i < FD_MAX_NUM; i++)
        {
            if (_rfds_array[i] == nullptr)
                continue;
            else
                std::cout << _rfds_array[i]->GetSockFd() << " ";
        }
        std::cout << std::endl;
    }
    ~SelectServer() {}

private:
    std::unique_ptr<Socket> _listensock;
    int _port;
    int _isrunning;
    Socket *_rfds_array[FD_MAX_NUM];
};