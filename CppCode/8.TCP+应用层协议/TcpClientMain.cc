#include "Protocol.hpp"
#include "Socket.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace Protocol;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage :\n\t" << argv[0] << "serverip sercerport" << std::endl;
        return 0;
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    NetWork::Socket *conn = new NetWork::TcpSocket();
    if (!conn->BuildConnectSockedMethod(serverip, serverport))
    {
        std::cerr << "connect " << serverip << ":" << serverport << " faild" << std::endl;
    }
    std::cerr << "connect " << serverip << ":" << serverport << " success" << std::endl;
    std::unique_ptr<Factory> factory = std::make_unique<Factory>();
    srand(time(nullptr));
    const std::string opers = "+-*/%__";
    while (true)
    {
        int x = rand() % 114;
        usleep(rand() % 2000);
        int y = rand() % 514;
        char oper = opers[rand() % opers.size()];
        std::shared_ptr<Request> req = factory->BuildRequest(x, y, oper);

        std::string requeststr;
        req->Serialize(&requeststr);
        std::cout << requeststr << std::endl;
        std::string testreq = requeststr;
        testreq += " ";
        testreq += "= ";

        requeststr = Encode(requeststr);
        std::cout << requeststr << std::endl;

        conn->Send(requeststr);
        std::string responsestr;
        while (true)
        {
            if (!conn->Recv(&responsestr, 1024))
                break;
            std::string response;
            if (!Decode(responsestr, &response))
                continue;
            auto resp = factory->BuildResponse();
            resp->Deserialize(response);

            std::cout << testreq << resp->GetResult() << "[" << resp->GetCode() << "]" << std::endl;
            break;
        }
        sleep(1);
    }
    conn->CloseSockFd();
    return 0;
}