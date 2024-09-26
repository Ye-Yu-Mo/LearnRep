#include <iostream>
#include <memory>
#include "SelectServer.hpp"

int main(int argc, char* argv[])
{
    std::cout<<"fd max number : "<<FD_MAX_NUM<<std::endl;
    uint16_t localport = std::stoi(argv[1]);
    std::shared_ptr<SelectServer> svr = std::make_shared<SelectServer>(localport);
    svr->InitServer();
    svr->Loop();
    return 0;
}