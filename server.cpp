/************* UDP SERVER CODE *******************/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>


using namespace std;


int udpSocket, nBytes;
uint8_t data[9];
struct sockaddr_in serverAddr, clientAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size, client_addr_size;
//
// char compute_checksum(char* str) {
//     char sum = 0x0;
//
//     int length = sizeof (str) / sizeof (char);
//     length--;
//     length--;
//     for (int i = 0; i < length; i++) {
//         sum += str[i];
//     }
//
//     return sum;
// }


uint8_t compute_checksum_7(uint8_t str[7]) {
    uint16_t sum = 0x0;

    for (int i = 0; i < 6; i++) {
        sum += (uint8_t) str[i];
        // printf("%02x\n", (uint8_t) str[i]);
    }

    uint8_t n = sum >> 8;

    sum = (uint8_t)  (sum + n);
    return (uint8_t) sum;
}


uint8_t compute_checksum_9(uint8_t str[9]) {
    uint16_t sum = 0x0;

    for (int i = 0; i < 8; i++) {
        sum += (uint8_t) str[i];
        // printf("%02x\n", (uint8_t) str[i]);
    }

    uint8_t n = sum >> 8;

    sum =  (sum + n);
    return (uint8_t) sum;
}


void sendACK(int nextSeqNum) {
    uint8_t segment[7];
    memset(segment,0x0, 7 * sizeof (uint8_t));
    segment[0] = 0x6;
    segment[4] = (nextSeqNum & 0x000000ff);
    segment[3] = (nextSeqNum & 0x0000ff00) >> 8;
    segment[2] = (nextSeqNum & 0x00ff0000) >> 16;
    segment[1] = (nextSeqNum & 0xff000000) >> 24;
    segment[5] = 0x0;
    segment[6] = compute_checksum_7(segment);

    // printf("\n Send: ");
    // for (int i = 0; i < 7; i++)
    //     printf("%02x ", segment[i]);
    // printf("\n");

    /*Send uppercase message back to client, using serverStorage as the address*/
    sendto(udpSocket, segment, 7, 0, (struct sockaddr *)&serverStorage, addr_size);
}


int getSeqNum(uint8_t byte[7]) {
    int n = 0;

    n = n + (byte[4] & 0x000000ff);
    n = n + ((byte[3] & 0x000000ff) << 8);
    n = n + ((byte[2] & 0x000000ff) << 16);
    n = n + ((byte[1] & 0x000000ff) << 24);

    return n;
}

int main(int argc, char* argv[]){
    int i, nextSeqNum = 0; //nextSeqNum = Largest Acceptable Frame
    int WINDOW_SIZE = atoi(argv[2]);
    uint8_t arr[WINDOW_SIZE];
    int port = atoi(argv[4]);
    memset(arr, 0x0, WINDOW_SIZE * sizeof (uint8_t));

    ofstream File(argv[1], std::ios::out);


    /*Create UDP socket*/
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("127.0.0.1"); // destination IP
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Bind socket with address struct*/
    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverStorage;

    while(1){

        /* Try to receive any incoming UDP datagram. Address and port of
          requesting client will be stored on serverStorage variable */
        nBytes = recvfrom(udpSocket, data, 9,0,(struct sockaddr *)&serverStorage, &addr_size);

        printf("\nReceive: ");
        for (int x = 0; x < 9; x++)
            printf("%02x ", data[x]);
        printf("\n");


        if (nextSeqNum < getSeqNum(data) + 1) { //Jika segment yang diterima lebih dari Largest Acceptable Frame
            if (compute_checksum_9(data) == data[8]) { // cek error dengan checksum

                //memasukkan karakter yang terbaca ke array dengan size WINDOW_SIZE
                arr[getSeqNum(data) - nextSeqNum] = data[6];

                //cek Largest Acceptable Frame
                for (i = 0; i < WINDOW_SIZE && arr[i] != 0x0; i++) {
                }
                for (int j = 0; j < i ;j++) {
                    // printf("%c", arr[j]);
                    File << arr[j];
                }

                //copy karakter yang belum terbaca pada sisa WINDOW
                uint8_t temp[WINDOW_SIZE];
                // strcpy(temp, arr);
                for (int j = 0; j < WINDOW_SIZE; j++) {
                }
                memset(arr, 0x0, WINDOW_SIZE * sizeof (uint8_t));
                for (int j = i ; j < WINDOW_SIZE; j++) {
                    arr[j-i] = arr[j];
                }

                // File << nextSeqNum << " " << i << endl;
                // update LAF
                nextSeqNum += i;

                sendACK(nextSeqNum);

            }
        }
        fflush(stdout);
        File.flush();
    }

    File.close();

    return 0;
}
