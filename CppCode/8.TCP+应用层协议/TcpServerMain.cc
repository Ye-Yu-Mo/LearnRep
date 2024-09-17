#include "Protocol.hpp"
#include "TcpServer.hpp"
#include "Calculate.hpp"
#include "Daemon.hpp"
#include <iostream>
#include <memory>
#include <unistd.h>

using namespace NetWork;
using namespace Protocol;
using namespace CalculateNS;

// 网络负责IO
// HandlerRequest负责字节流数据解析和调用业务
std::string HandlerRequest(std::string &inbufferstream, bool *error_code)
{
    *error_code = true;
    // 业务对象
    Calculate calculte;

    // 工厂对象，构建请求对象
    std::unique_ptr<Factory> factory = std::make_unique<Factory>();
    auto req = factory->BuildRequest();

    // 分析字节流，查看是否报文是否完整
    std::string total_resp_string;
    std::string message;
    while (Decode(inbufferstream, &message))
    {
        std::cout << message << "----messgae" << std::endl;
        // 读取完整报文，进行反序列化
        if (!req->Deserialize(message))
        {
            std::cout << "Deserialize error" << std::endl;
            *error_code = false;
            return std::string();
        }
        std::cout << "Deserialize success" << std::endl;
        // 处理业务
        auto resp = calculte.Cal(req);
        // 序列化响应结果
        std::string send_string;
        resp->Serialize(&send_string);
        // 构建响应字符串
        send_string = Encode(send_string);
        // 发送
        total_resp_string += send_string;
    }
    return total_resp_string;
}

int main(int argc, char *argv[])
{
    // if (argc != 2)
    // {
    //     std::cout << "Usage :\n\t" << argv[0] << " port" << std::endl;
    //     return 0;
    // }
    // uint16_t localport = std::stoi(argv[1]);

    uint16_t localport = 8888;
    // Fork 子进程
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Fork failed." << std::endl;
        return 1;
    }
    else if (pid > 0)
    {
        // 父进程退出
        return 0;
    }
    Daemon(false, false);
    std::unique_ptr<TcpServer> svr(new TcpServer(localport, HandlerRequest));
    svr->Loop();
    return 0;
}