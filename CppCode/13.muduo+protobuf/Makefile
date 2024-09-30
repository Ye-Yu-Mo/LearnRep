all: Server Client
Client: ProtobufClient.cc Request.pb.cc ./protobuf/codec.cc
	g++ $^ -o $@ -I./muduo/include -L./muduo/lib -lmuduo_net -lmuduo_base -lprotobuf -lz
Server: ProtobufServer.cc Request.pb.cc ./protobuf/codec.cc
	g++ $^ -o $@ -I./muduo/include -L./muduo/lib -L~/Download/jsoncpp/build/bin -lmuduo_net -lmuduo_base -Wall -lcurl -lssl -lcrypto -ljsoncpp -lprotobuf -lz

.PHONY:clean
clean:
	rm -f Client Server