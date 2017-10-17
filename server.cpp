/************* UDP SERVER CODE *******************/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <stdlib.h>


int udpSocket, nBytes;
char data[9];
struct sockaddr_in serverAddr, clientAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size, client_addr_size;

char compute_checksum(char* str) {
    char sum = 0x0;

    int length = sizeof (str) / sizeof (char);
    length--;
    length--;
    for (int i = 0; i < length; i++) {
        sum += str[i];
    }

    return sum;
}

void sendACK(int nextSeqNum) {
    char segment[7];
    memset(segment,0x0, sizeof segment);
    segment[0] = 0x6;
    segment[4] = (nextSeqNum & 0x000000ff);
    segment[3] = (nextSeqNum & 0x0000ff00) >> 8;
    segment[2] = (nextSeqNum & 0x00ff0000) >> 16;
    segment[1] = (nextSeqNum & 0xff000000) >> 24;
    segment[5] = 0x0;
    segment[6] = compute_checksum(segment);

    /*Send uppercase message back to client, using serverStorage as the address*/
    sendto(udpSocket, segment, 7, 0, (struct sockaddr *)&serverStorage, addr_size);
}


int getSeqNum(char byte[7]) {
    int n = 0;

    n = n + (byte[4] & 0x000000ff);
    n = n + ((byte[3] & 0x000000ff) << 8);
    n = n + ((byte[2] & 0x000000ff) << 16);
    n = n + ((byte[1] & 0x000000ff) << 24);

    return n;
}

int main(int argc, char* argv[]){
    int i, nextSeqNum = 0; //nextSeqNum = Largest Acceptable Frame
    int WINDOW_SIZE = 4;
    char arr[WINDOW_SIZE];
    memset(arr, 0x0, sizeof arr);

    /*Create UDP socket*/
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Bind socket with address struct*/
    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverStorage;

    // printf("c");

    while(1){
        /* Try to receive any incoming UDP datagram. Address and port of
          requesting client will be stored on serverStorage variable */
        nBytes = recvfrom(udpSocket, data, 9,0,(struct sockaddr *)&serverStorage, &addr_size);

        // for (int x = 0; x < 9; x++)
        //     printf("%02x ", data[x]);
        // printf("\n");

        if (nextSeqNum < getSeqNum(data) + 1) { //Jika segment yang diterima lebih dari Largest Acceptable Frame
            if (compute_checksum(data) == data[8]) { // cek error dengan checksum

                //memasukkan karakter yang terbaca ke array dengan size WINDOW_SIZE
                arr[getSeqNum(data) - nextSeqNum] = data[6];

                //cek Largest Acceptable Frame
                for (i = 0; i < WINDOW_SIZE && arr[i] != 0x0; i++) {
                }
                for (int j = 0; j < i ;j++) {
                    printf("%c", arr[j]);
                }

                fflush(stdout);

                //copy karakter yang belum terbaca pada sisa WINDOW
                char temp[WINDOW_SIZE];
                strcpy(temp, arr);
                memset(arr, 0x0, sizeof arr);
                for (int j = i ; j < WINDOW_SIZE; j++) {
                    arr[j-i] = arr[j];
                }

                // update LAF
                nextSeqNum += i;

                sendACK(nextSeqNum);
            }
        }
    }

    return 0;
}
