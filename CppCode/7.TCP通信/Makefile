.PHONY:all
all:tcp_server tcp_client

tcp_client:tcpclient.cc 
	g++ $^ -std=c++11 -o $@ 
tcp_server:main.cc 
	g++ $^ -std=c++11 -o $@
.PHONY:clean
clean:
	rm -rf tcp_client tcp_server 
