#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include "TcpServer.hpp"
#include "HttpProtocol.hpp"

std::string HanlderHttpRequest(std::string &request)
{
    HttpRequest req;
    req.Deserialize(request);
    req.DebugHttp();

    std::string content = "<html><h1>hello http</h1></html>";
    std::string httpStatusline = "Http/1.0 200 OK\r\n";
    std::string httpheader = "Content-Length: " + std::to_string(content.size()) + "\r\n";
    httpheader += "\r\n";

    std::string httpresponse = httpStatusline + httpheader + content;
    return httpresponse;
}

int main(int argc, char *argv[])
{
    // if(argc!=2)
    // {
    //     std::cout<<"Usage:\n\t"<<argv[0]<<"port\n\n";
    //     return 0;
    // }
    // uint16_t localport = std::stoi(argv[1]);
    uint16_t localport = 8888;
    std::unique_ptr<TcpServer> svr(new TcpServer(localport, HanlderHttpRequest));
    svr->Loop();
    return 0;
}
