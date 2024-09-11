#include <iostream>
#include <cerrno>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void Usage (const std::string &proc)
{
    std::cout<<"Usage:\n\t"<<proc<<"server_ip server_port" <<std::endl;
}

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        return 1;    
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    // 创建socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0)
    {
        perror("socket创建失败");
        exit(errno);
    }
    std::cout<<"socket创建成功,sockfd:"<<sockfd<<std::endl;

    // 客户机也需要绑定，但是不需要主动调用，客户机会在第一次发送数据时自动绑定数据
    // 填充服务器信息
    struct sockaddr_in server;
    memset(&server, 0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    while(true)
    {
        // 发送的数据
        std::string inbuffer;
        std::cout<<"Please Enter# ";
        std::getline(std::cin, inbuffer);
        // 发送消息（请求）
        ssize_t n = sendto(sockfd, inbuffer.c_str(), inbuffer.size(), 0, (struct sockaddr*)&server, sizeof(server));
        // 收消息
        if(n>0)
        {
            char buffer[1024];
            struct sockaddr_in tmp;
            socklen_t len = sizeof(tmp);
            ssize_t m = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&tmp, &len);
            if(m>0)
            {
                buffer[m] = '\0';
                std::cout<<"server echo# "<<buffer<<std::endl;
            }
            else
                break;
        }
        else
            break;
    }   
    close(sockfd);
    return 0;
}