// tcp_client.cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include "Socket.hpp"

#define PORT 8888

int main()
{
    NetWork::TcpSocket send;
    std::string s = "127.0.0.1";
    send.BuildConnectSockedMethod(s, 8888);
    int i = 10;
    while (i--)
    {
        std::string str = "3707764736{        'format_msg' :         {                'ctime' : 1729065220,                'file' : 'servertest.cc',                'level' : 'FATAL',                'line' : 20,                'logger' : 'synclogger',                'payload' : '...fatal...',                'tid' : '139794736060224'        },        'msg_mod' : 'format'}";
        send.Send(str);
    }
    return 0;
}
