#include "udpserver.hpp"
#include "udpclient.hpp"
#include "threadpool.hpp"
#include <memory>
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void Usage(std::string proc) // 使用提示
{
    std::cout << "Usage:\n\t" << proc << " other_ip\n"
              << std::endl;
}

void client(const std::string &server_ip)
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
    UdpClient::ptr client = std::make_unique<UdpClient>(server_ip); // 客户端线程
    client->Start();
}

void server()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []
            { return ready; });
    UdpServer::ptr server = std::make_unique<UdpServer>(); // 服务端线程
    server->Init();
    server->Start();

}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    threadpool::ptr pool = std::make_unique<threadpool>(); // 线程池管理
    pool->push(client, argv[1]);
    pool->push(server);
    return 0;
}