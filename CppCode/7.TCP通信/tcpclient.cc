#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define CONV(addr_ptr) ((struct sockaddr *)addr_ptr)
void Usage(const std::string& proc)
{
  std::cout << "Usage:\n\t" << proc <<" server_ip server_port" << std::endl;
}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    Usage(argv[0]);
    return -1;
  }
  std::string serverip = argv[1];
  uint16_t serverport = std::stoi(argv[2]);
      
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd<0)
  {
    std::cout << "socket error" << std::endl;
    return -1;
  }
  
  // connect
  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(serverport);
  
  inet_pton(AF_INET, serverip.c_str(), &server.sin_addr);
  int n = connect(sockfd, CONV(&server), sizeof(server));
  if(n<0)
  {
    perror("connect error");
    return 2;
  }
  while(true)
  {
    std::string inbuffer;
    std::cout << "please enter# " << std::endl;
    getline(std::cin, inbuffer);
    
    ssize_t n =write(sockfd, inbuffer.c_str(), inbuffer.size());
    if(n>0)
    {
      char buffer[1024];
      ssize_t m = read(sockfd, buffer, sizeof(buffer)-1);
      if(m>0)
      {
        buffer[m]='\0';
        std::cout<<"get a ech message -> "<<buffer<<std::endl;
      }
    }
  }
  close(sockfd);
  return 0;
}

