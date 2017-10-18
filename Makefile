all: client server

client: client.o
	g++ client.cpp -o sendfile

server: server.o
	g++ server.cpp -o recvfile
