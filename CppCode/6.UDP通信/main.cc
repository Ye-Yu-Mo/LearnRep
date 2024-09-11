#include "udpserver.hpp"
#include <memory>

void Usage(std::string proc) // 使用提示
{
    std::cout << "Usage:\n\t" << proc << "local_port\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port);
    usvr->Init();
    usvr->Start();
    return 0;
}