#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Convert(addr_ptr) ((struct sockaddr *)addr_ptr)

namespace NetWork
{
    constexpr int DEFAULT_SOCKFD = -1;
    constexpr int DEFAULT_BACKLOG = 128;
    class Socket
    {
    public:
        virtual ~Socket() {}
        virtual void CreateSocketOrDie() = 0;
        virtual void BindSocketOrDie(uint16_t port) = 0;
    };

    class UdpSocket : public Socket
    {
    public:
        UdpSocket(int sockfd = DEFAULT_SOCKFD) : _sockfd(sockfd) {}
        ~UdpSocket() {}
        void SendTo(const std::string &send_str, const std::string &dest_ip, uint16_t dest_port)
        {
            struct sockaddr_in dest_addr;
            memset(&dest_addr, 0, sizeof(dest_addr));
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_addr.s_addr = inet_addr(dest_ip.c_str());
            dest_addr.sin_port = htons(dest_port);
            ssize_t ret = sendto(_sockfd, send_str.c_str(), send_str.size(), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (ret < 0)
                throw std::runtime_error("sendto 失败: " + std::string(strerror(errno)));
        }
        ssize_t RecvFrom(std::string *buffer, size_t size, std::string *src_ip, uint16_t *src_port)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            return recvfrom(_sockfd, buffer, size, 0, Convert(&peer), &len);
        }

    private:
        void CreateSocketOrDie() override
        {
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (_sockfd < 0)
                throw std::runtime_error("Socket 创建失败: " + std::string(strerror(errno)));
        }
        void BindSocketOrDie(uint16_t port) override
        {
            struct sockaddr_in local;
            bzero(&local, sizeof(local));       // memset
            local.sin_family = AF_INET;         // 协议簇
            local.sin_port = htons(port);       // 端口号
            local.sin_addr.s_addr = INADDR_ANY; // ip地址
            int n = ::bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
            if (n != 0)
                throw std::runtime_error("Bind 失败: " + std::string(strerror(errno)));
        }

    public:
        void CreateBuildSocketMethod(uint16_t port)
        {
            CreateSocketOrDie();
            BindSocketOrDie(port);
        }

    private:
        int _sockfd;
    };

    class TcpSocket : public Socket
    {
    public:
        TcpSocket(int sockfd = DEFAULT_SOCKFD) : _sockfd(sockfd) {}
        ~TcpSocket() {}

    public:
        void BuildListenSocketMethod(uint16_t port, int backlog = DEFAULT_BACKLOG)
        {
            CreateSocketOrDie();
            BindSocketOrDie(port);
            ListenSocketOrDie(backlog);
        }
        bool BuildConnectSockedMethod(std::string &serverip, uint16_t serverport)
        {
            CreateSocketOrDie();
            return ConnectServer(serverip, serverport);
        }
        void BuildNormalSockMethod(int sockfd)
        {
            SetSockFd(sockfd);
        }
        int GetSockFd() { return _sockfd; }
        void SetSockFd(int sockfd) { _sockfd = sockfd; }
        void CloseSockFd()
        {
            if (_sockfd > DEFAULT_SOCKFD)
                ::close(_sockfd);
        }
        bool Recv(std::string *buffer, int size = 1024 * 10)
        {
            char inbuffer[size];
            ssize_t n = recv(_sockfd, inbuffer, size - 1, 0);
            std::cout << "recv 创建成功" << std::endl;
            if (n > 0)
            {
                inbuffer[n] = 0;
                *buffer += inbuffer;
                return true;
            }
            return false;
        }
        void Send(const std::string &send_str)
        {
            ssize_t ret = send(_sockfd, send_str.c_str(), send_str.size(), 0);
            if (ret < 0)
                throw std::runtime_error("send 失败: " + std::string(strerror(errno)));
        }
        TcpSocket *AcceptConnection(std::string *peerip, uint16_t *peerport)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int newsockfd = ::accept(_sockfd, Convert(&peer), &len);
            if (newsockfd < 0)
                return nullptr;
            *peerport = ntohs(peer.sin_port);
            *peerip = inet_ntoa(peer.sin_addr);
            TcpSocket *s = new TcpSocket(newsockfd);
            return s;
        }

    private:
        void CreateSocketOrDie() override
        {
            _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
                throw std::runtime_error("Socket 创建失败！" + std::string(strerror(errno)));
            std::cout << "socket 创建成功" << std::endl;
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
                throw std::runtime_error("Bind 失败" + std::string(strerror(errno)));
            std::cout << "bind 创建成功" << std::endl;

        }
        void ListenSocketOrDie(int backlog = DEFAULT_BACKLOG)
        {
            int n = ::listen(_sockfd, backlog);
            if (n < 0)
                throw std::runtime_error("listen 失败" + std::string(strerror(errno)));
            std::cout << "listen 创建成功" << std::endl;
            
        }

        bool ConnectServer(std::string &serverip, uint16_t serverport)
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

    private:
        int _sockfd;
    };
}