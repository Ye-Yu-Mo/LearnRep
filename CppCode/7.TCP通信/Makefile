.PHONY:all
all:tcp_server tcp_client

tcp_client:tcpclient.cc 
	g++ $^ -std=c++14 -o $@ 
tcp_server:main.cc 
	g++ $^ -std=c++14 -o $@ -pthread
.PHONY:clean
clean:
	rm -rf tcp_client tcp_server 
