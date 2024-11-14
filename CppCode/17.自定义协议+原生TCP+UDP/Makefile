.PHONY:all
all:test server client
server:TCPserver.cc
	g++ $^ -o $@ -std=c++14
client:TCPclient.cc
	g++ $^ -o $@ -std=c++14

test:test.cc
	g++ $^ -o $@ -std=c++11 -ljsoncpp
.PHONY:clean
clean:
	rm -rf test server test