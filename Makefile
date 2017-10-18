all: client.o server.o
	g++ client.cpp -o sendfile
	g++ server.cpp -o recvfile

clean:
	\rm *.o sendfile recvfile
