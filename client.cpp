/************* UDP CLIENT CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int clientSocket, portNum, nBytes;
char data[1024];
struct sockaddr_in serverAddr;
socklen_t addr_size;

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

void sendSegment(char c, int seqNum) {

    char segment[9];
    memset(segment, 0x0, sizeof segment);
    segment[0] = 0x1;
    segment[4] = (seqNum & 0x000000ff);
    segment[3] = (seqNum & 0x0000ff00) >> 8;
    segment[2] = (seqNum & 0x00ff0000) >> 16;
    segment[1] = (seqNum & 0xff000000) >> 24;
    segment[5] = 0x2;
    segment[6] = c;
    segment[7] = 0x3;
    segment[8] = compute_checksum(segment);

    /*Send message to server*/
    sendto(clientSocket,segment, 9, 0, (struct sockaddr *)&serverAddr, addr_size);
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

    /*Create UDP socket*/
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverAddr;

    char recv_data[7];

    while(1){
        printf("Type a sentence to send to server:\n");
        fgets(data,1024,stdin);
        printf("You typed: %s",data);

        // nBytes = strlen(data) + 1;
        int seqNum = 0;
        int LAR = 0; //last ACK received
        int WINDOW_SIZE = 4;
        while (seqNum < strlen(data)) {
            int i = LAR;
            for (i; i < LAR + WINDOW_SIZE; i++) {
                sendSegment(data[i], i);
                // printf("%c", data[i]);
            }

            memset(recv_data, 0x0, sizeof recv_data);
            /*Receive message from server*/
            nBytes = recvfrom(clientSocket,recv_data, 7, 0, NULL, NULL);

            // printf("%02x\n", compute_checksum(recv_data));
            for (int i = 0; i < 7; i++)
                printf("%02x ", recv_data[i]);

            printf("%02x ", compute_checksum(recv_data));
            fflush(stdout);

            printf("\n");

            if (compute_checksum(recv_data) == recv_data[6]) { //cek checksum
                seqNum = getSeqNum(recv_data);
                LAR = seqNum;
            }
        }
    }

    return 0;
}
