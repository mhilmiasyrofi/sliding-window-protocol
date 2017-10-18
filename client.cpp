/************* UDP CLIENT CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>


using namespace std;

int clientSocket, portNum, nBytes;
char data[1000000];
struct sockaddr_in serverAddr;
socklen_t addr_size;
int WINDOW_SIZE = 4;

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

    sum = (sum + n);
    return (uint8_t) sum;
}


uint8_t compute_checksum_9(uint8_t str[9]) {
    uint16_t sum = 0x0;

    for (int i = 0; i < 8; i++) {
        sum += (uint8_t) str[i];
        // printf("%02x\n", (uint8_t) str[i]);
    }

    uint8_t n = sum >> 8;

    sum = (sum + n);
    return (uint8_t) sum;
}

void sendSegment(char c, int seqNum) {

    uint8_t segment[9];
    memset(segment, 0x0, sizeof segment);
    segment[0] = 0x1;
    segment[4] = (seqNum & 0x000000ff);
    segment[3] = (seqNum & 0x0000ff00) >> 8;
    segment[2] = (seqNum & 0x00ff0000) >> 16;
    segment[1] = (seqNum & 0xff000000) >> 24;
    segment[5] = 0x2;
    segment[6] = (uint8_t) c;
    segment[7] = 0x3;
    segment[8] = compute_checksum_9(segment);

    // printf("\n Send: ");
    // for (int i = 0; i < 9; i++) {
    //     printf("%02x ",segment[i]);
    // }
    // printf("\n");

    /*Send message to server*/
    sendto(clientSocket,segment, 9, 0, (struct sockaddr *)&serverAddr, addr_size);
}

int getSeqNum(uint8_t byte[7]) {
    int n = 0;

    n = n + (byte[4] & 0x000000ff);
    n = n + ((byte[3] & 0x000000ff) << 8);
    n = n + ((byte[2] & 0x000000ff) << 16);
    n = n + ((byte[1] & 0x000000ff) << 24);

    return n;
}

void readFile(char* FileName) {

    string STRING;
    ifstream infile;
    infile.open (FileName);
    while(!infile.eof()) // To get you all the lines.
    {
        getline(infile,STRING); // Saves the line in STRING.
        // cout << STRING; // Prints our STRING.
        strcat(data, STRING.c_str());
        if (!infile.eof())
            strcat(data, "\n");
    }
    infile.close();
}

int main(int argc, char* argv[]){

    /*Create UDP socket*/
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    int port = atoi(argv[5]);
    int LAR = 0; //last ACK received

    WINDOW_SIZE = atoi(argv[2]);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(argv[4]);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverAddr;

    uint8_t  recv_data[7];

    readFile(argv[1]);
    // printf("You typed: \n%s",data);

    while (LAR < strlen(data)){

        // nBytes = strlen(data) + 1;
        int seqNum = 0;

        while (seqNum < strlen(data)) {
            int i = LAR;
            for (i; i < LAR + WINDOW_SIZE; i++) {
                sendSegment(data[i], i);
                // printf("%02x", data[i]);
            }

            memset(recv_data, 0x0, 7 * sizeof (uint8_t));
            /*Receive message from server*/
            nBytes = recvfrom(clientSocket,recv_data, 7, 0, NULL, NULL);


            printf("\n Receive: ");
            for (int i = 0; i < 7; i++)
                printf("%02x ", recv_data[i]);
            printf("\n");

            // printf("%02x ", compute_checksum_7(recv_data));

            printf("LAR: %d\n", LAR );
            printf("Data: %d\n", (int) strlen(data));
            printf("\n");


            if (compute_checksum_7(recv_data) == recv_data[6]) { //cek checksum
                seqNum = getSeqNum(recv_data);
                if (seqNum > LAR)
                    LAR = seqNum;
            }

        }
        fflush(stdout);
    }

    return 0;
}
