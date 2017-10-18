all: src/client.o src/server.o
	g++ src/client.cpp -o sendfile
	g++ src/server.cpp -o recvfile

clean:
	\rm sendfile recvfile
	\rm src/*.o
