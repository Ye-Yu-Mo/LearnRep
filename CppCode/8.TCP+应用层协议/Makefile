.PHONY:all
all:tcpclient tcpserver

LDFLAG=#-DSelfDefine=1

tcpclient:TcpClientMain.cc
	x86_64-w64-mingw32-g++ $^ -g -o $@ $(LDFLAG)  -std=c++14  -ljsoncpp -static
tcpserver:TcpServerMain.cc
	x86_64-w64-mingw32-g++ $^ -g -o $@ $(LDFLAG)  -std=c++14 -lpthread -ljsoncpp -static

.PHONY:clean
clean:
	rm -f tcpclient tcpserver