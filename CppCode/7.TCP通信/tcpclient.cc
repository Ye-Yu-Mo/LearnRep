#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "comm.hpp"

using namespace std;

#define Retry_Count 5

void handler(int signo)
{
    cout << "signo: " << signo << endl;
    exit(0);
}

void Usage(const string &process)
{
    cout << "Usage: \n\t" << process << "server_ip server_port" << endl;
}

bool visitServer(string &serverip, uint16_t &serverport, int *cnt)
{
    string inbuffer;
    char service_list[1024];
    ssize_t m = 0;
    ssize_t n = 0;
    bool ret = true;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "socket错误\n";
        ret = false;
        goto END;
    }
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    inet_pton(AF_INET, serverip.c_str(), &server.sin_addr);
    n = connect(sockfd, CONV(&server), sizeof(server));

    if (n < 0)
    {
        cerr << "连接错误\n";
        ret = false;
        goto END;
    }
    *cnt = 0;

    m = read(sockfd, service_list, sizeof(service_list) - 1);
    if (m > 0)
    {
        service_list[m] = 0;
        cout << "服务列表为:" << service_list << endl;
    }

    cout << "你选择的服务是# ";
    getline(cin, inbuffer);
    write(sockfd, inbuffer.c_str(), inbuffer.size());
    cout << "Enter> ";
    getline(cin, inbuffer);
    if (inbuffer == "quit")
        return true;

    n = write(sockfd, inbuffer.c_str(), inbuffer.size());
    if (n > 0)
    {
        char buffer[1024];
        m = read(sockfd, buffer, inbuffer.size() - 1);
        if (m > 0)
        {
            buffer[m] = 0;
            cout << buffer << endl;
        }
        else if (m == 0)
        {
            return true;
        }
        else
        {
            ret = false;
            goto END;
        }
    }
    else
    {
        cout << "写入错误\n";
        ret = false;
        goto END;
    }
END:
    close(sockfd);
    return ret;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    string serverip = argv[1];
    uint16_t serverport = stoi(argv[2]);

    signal(SIGPIPE, SIG_IGN);

    int cnt = 1;
    while (cnt <= Retry_Count)
    {
        bool result = visitServer(serverip, serverport, &cnt);
        if (result)
            break;
        else
        {
            sleep(1);
            cout << "TCP连接失败,正在重新连接,重连次数:" << cnt << endl;
            cnt++;
        }
    }
    if (cnt >= Retry_Count)
        cout << "连接超时\n";
    return 0;
}