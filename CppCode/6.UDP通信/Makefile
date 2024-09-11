.PHONY:all
all:udp_server udp_client

udp_server:main.cc
	g++ $^ -std=c++14 -o $@ 
udp_client:udpclient.cc
	g++ $^ -std=c++14 -o $@

.PHONY:clean
clean:
	rm -f udp_client udp_server