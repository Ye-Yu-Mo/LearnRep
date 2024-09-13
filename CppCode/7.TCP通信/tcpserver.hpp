#pragma once 
#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "nocopy.hpp"

// macro func forced conversion
#define CONV(addr_ptr) ((struct sockaddr *)addr_ptr)
const static int DEFAULT_BACKLOG = 1; // TODO

class TcpServer : public nocopy{
public:
  TcpServer(uint16_t port)
    : _port(port), _is_running(false)
  {}
  void Init()
  {
    // create socket 
    _listen_sock = socket(AF_INET, SOCK_STREAM, 0); // IPv4 TCP
    if (_listen_sock < 0)
    {
      perror("create socfet fail!");
      exit(-1);
    }
    int opt = 1;
    setsockopt(_listen_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    std::cout << "create seocket success! socketfd:" << _listen_sock << std::endl;
    
    // fill local network info and bind
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_listen_sock, CONV(&local), sizeof(local))!=0)
    {
      perror("bind fail!");
      exit(-2);
    }

    // TCP setting of socket to listn state
    if(listen(_listen_sock, DEFAULT_BACKLOG)!=0)
    {
      perror("listen fail!");
      exit(-3);
    }
    std::cout << "listen success, sockfd:" << _listen_sock <<std::endl;
  }
  // TCP is full deplex communiaction
  void Service(int sockfd)
  {
    char buffer[1024];
    while(true)
    {
      ssize_t n = read(sockfd, buffer, sizeof(buffer)-1);
      if(n>0)
      {
        buffer[n] = '\0';
        std::cout << "client say# " << buffer << std::endl;
        std::string echo_string = "server echo# ";
        echo_string += buffer;
        write(sockfd, echo_string.c_str(), echo_string.size());
      }
      else if (n==0) // file end or connect close lose
      {
        std::cout << "client quit ..." << std::endl;
        break;
      }
      else // read fail! 
      {
        std::cout << "read fail!" << std::endl;
        break;
      }  
    }
  }
  void Start()
  {
    _is_running = true;
    while(_is_running)
    {
      sleep(1);
    }
  }
  ~TcpServer()
  {}

private:
  uint16_t _port;
  int _listen_sock;
  bool _is_running;
};
