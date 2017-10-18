#include <bits/stdc++.h>
#include <stdint.h>

using namespace std;

uint8_t compute_checksum(char str[2]) {
    uint16_t sum = 0x0;

    // int length = sizeof (str) / sizeof (char);
    // length--;
    // length--;
    for (int i = 0; i < 2; i++) {
        sum += (uint8_t) str[i];
        printf("%02x\n", (uint8_t) str[i]);
    }

    uint8_t n = sum >> 8;

    printf("%02x\n", sum);
    printf("%02x\n", n);

    sum = (uint8_t)  (sum + n);
    return sum;
}


int main() {

    char str[2];
    str[0] = 0x06;
    str[1] = 0x0b;

    printf("%02x\n", compute_checksum(str));

    return 0;
}
