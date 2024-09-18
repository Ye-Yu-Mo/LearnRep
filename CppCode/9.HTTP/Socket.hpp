#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Convert(addrptr) ((struct sockaddr *)addrptr)

namespace NetWork
{
    const static int defaultsockfd = -1;
    const int backlog = 5;

    enum
    {
        SocketError = 1,
        BindError,
        ListenError
    };
    class Socket
    {
    public:
        virtual ~Socket() {}
        virtual void CreateSocketOrDie() = 0;
        virtual void BindSocketOrDie(uint16_t port) = 0;
        virtual void ListenSocketOrDie(int backlog) = 0;
        virtual Socket *AcceptConnection(std::string *peerip, uint16_t *peerport) = 0;
        virtual bool ConnectServer(std::string &serverip, uint16_t serverport) = 0;
        virtual int GetSockFd() = 0;
        virtual void SetSockFd(int sockfd) = 0;
        virtual void CloseSockFd() = 0;
        virtual bool Recv(std::string *buffer, int size) = 0;
        virtual void Send(std::string &send_str) = 0;

    public:
        void BuildListenSocketMethod(uint16_t port, int backlog)
        {
            CreateSocketOrDie();
            BindSocketOrDie(port);
            ListenSocketOrDie(backlog);
        }
        bool BuildConnectSocketMethod(std::string &serverip, uint16_t serverport)
        {
            CreateSocketOrDie();
            return ConnectServer(serverip, serverport);
        }
        void BuildNormalSockMethod(int sockfd)
        {
            SetSockFd(sockfd);
        }
    };

    class TcpSocket : public Socket
    {
    public:
        TcpSocket(int sockfd = defaultsockfd)
            : _sockfd(sockfd)
        {
        }
        ~TcpSocket() {}
        void CreateSocketOrDie() override
        {
            _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
                exit(SocketError);
        }
        void BindSocketOrDie(uint16_t port) override
        {
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = INADDR_ANY;
            local.sin_port = htons(port);

            int n = ::bind(_sockfd, Convert(&local), sizeof(local));
            if (n < 0)
                exit(BindError);
        }
        void ListenSocketOrDie(int backlog) override
        {
            int n = ::listen(_sockfd, backlog);
            if (n < 0)
                exit(ListenError);
        }
        Socket *AcceptConnection(std::string *peerip, uint16_t *peerport) override
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int newsockfd = ::accept(_sockfd, Convert(&peer), &len);
            if (newsockfd < 0)
                return nullptr;
            *peerport = ntohs(peer.sin_port);
            *peerip = inet_ntoa(peer.sin_addr);
            Socket *s = new TcpSocket(newsockfd);
            return s;
        }
        bool ConnectServer(std::string &serverip, uint16_t serverport) override
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(serverip.c_str());
            server.sin_port = htons(serverport);

            int n = ::connect(_sockfd, Convert(&server), sizeof(server));
            if (n == 0)
                return true;
            else
                return false;
        }
        int GetSockFd() override
        {
            return _sockfd;
        }
        void SetSockFd(int sockfd) override
        {
            _sockfd = sockfd;
        }
        void CloseSockFd() override
        {
            if (_sockfd > defaultsockfd)
                ::close(_sockfd);
        }
        bool Recv(std::string *buffer, int size) override
        {
            char inbuffer[size];
            ssize_t n = recv(_sockfd, inbuffer, size - 1, 0);
            if (n > 0)
            {
                inbuffer[n] = 0;
                *buffer += inbuffer;
                return true;
            }
            return false;
        }
        void Send(std::string &send_str) override
        {
            send(_sockfd, send_str.c_str(), send_str.size(), 0);
        }

    private:
        int _sockfd;
    };
}