.PHONY:all
all:DictClient DictServer

DictClient:DictClient.cc
	g++ $^ -o $@ -I./muduo/include -L./muduo/lib -lmuduo_net -lmuduo_base
DictServer:DictServer.cc
	g++ $^ -o $@ -I./muduo/include -L./muduo/lib -L~/Download/jsoncpp/build/bin -lmuduo_net -lmuduo_base -Wall -lcurl -lssl -lcrypto -ljsoncpp

.PHONY:clean
clean:
	rm -f DictServer DictClient