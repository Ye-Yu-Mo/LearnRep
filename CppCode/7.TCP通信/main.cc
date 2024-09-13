#include <memory>
#include <string>

#include "tcpserver.hpp"

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void Usage(std::string proc)
{
  std::cout << "Usage:\n\t" << proc << "local_prot\n" << std::endl;
}

int main(int argc, char *argv[])
{
  if(argc!=2)
  {
    Usage(argv[0]);
    return -1;
  }
  uint16_t port = std::stoi(argv[1]);
  std::unique_ptr<TcpServer> tcpserver = make_unique<TcpServer>(port);
  tcpserver->Init();
  tcpserver->Start();
  return 0;
}
