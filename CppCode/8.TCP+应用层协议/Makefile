.PHONY:all
all:tcpclient tcpserver

LDFLAG=#-DSelfDefine=1

tcpclient:TcpClientMain.cc
	g++ $^ -g -o $@ $(LDFLAG)  -std=c++14  -ljsoncpp
tcpserver:TcpServerMain.cc
	g++ $^ -g -o $@ $(LDFLAG)  -std=c++14 -lpthread -ljsoncpp

.PHONY:clean
clean:
	rm -f tcpclient tcpserver