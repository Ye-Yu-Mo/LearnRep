#include "tcpserver.hpp"
#include "comm.hpp"
#include "Translate.hpp"
#include "Deamon.hpp"
#include <memory>
#include <cctype>
#include <algorithm>

using namespace std;

void Usage(std::string proc)
{
    std::cout << "Usage : \n\t" << proc << " local_port\n"
              << std::endl;
}

Translate trans;

void Interact(int sockfd, std::string &out, const std::string &in)
{
    char buffer[1024];
    ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
    if (n > 0)
    {
        buffer[n] = 0;
        write(sockfd, in.c_str(), in.size());
    }
    else if (n == 0) // read如果返回值是0，表示读到了文件结尾(对端关闭了连接！)
    {
    }
    else
    {
    }
}

// 未来的服务器是部署在你的云服务器上的，你怎么知道你的服务未来在任何一个时刻，都是健康的呢？？
// 我可以定期(30s)向我的服务器发送最小服务请求，如果得到了回复，说明我们的服务是正常的.
// 这个机制，我们称之为心跳机制.我们可以问client->服务器, 反向的 server->client
void Ping(int sockfd, InetAddr addr)
{
    // 一直进行IO
    std::string message;
    Interact(sockfd, message, "Pong");
}

void Translate(int sockfd, InetAddr addr)
{

    // std::string word;
    char wordbuf[128];
    int n = read(sockfd, wordbuf, sizeof(wordbuf) - 1);
    if (n > 0)
        wordbuf[n] = 0;
    std::string chinese = trans.Excute(wordbuf);
    write(sockfd, chinese.c_str(), chinese.size());
}

// 改成大写，字符串改成大写
void Transform(int sockfd, InetAddr addr)
{
    char message[128];
    int n = read(sockfd, message, sizeof(message) - 1); // tcp中要正确的读取，必须结合用户协议！！
    if (n > 0)
        message[n] = 0;
    std::string messagebuf = message;

    std::transform(messagebuf.begin(), messagebuf.end(), messagebuf.begin(), [](unsigned char c)
                   { return std::toupper(c); });
    write(sockfd, messagebuf.c_str(), messagebuf.size());
}

// ./tcp_server 8888
int main(int argc, char *argv[])
{
    // TCP代码中，我们在编写IO代码的时候，尤其是网络IO的时候，用的read，write，目前的代码其实都是有BUG的！！
    if (argc != 2)
    {
        Usage(argv[0]);
        return Usage_Err;
    }
    uint16_t port = stoi(argv[1]);
    Deamon(false, false);
    std::unique_ptr<TcpServer> tsvr = make_unique<TcpServer>(port);

    tsvr->RegisterFunc("ping", Ping);
    tsvr->RegisterFunc("translate", Translate);
    tsvr->RegisterFunc("transform", Transform);
    tsvr->Init();

    tsvr->Start();

    return 0;
}