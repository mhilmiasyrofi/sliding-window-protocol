#include <bits/stdc++.h>

using namespace std;

int main() {

    char byte[5];

    memset(byte, 0x0, sizeof byte);

    byte[3] = 0x1;

    int n = 0;

    n = n + (byte[4] & 0x000000ff);
    n = n + ((byte[3] & 0x000000ff) << 8);
    n = n + ((byte[2] & 0x000000ff) << 16);
    n = n + ((byte[1] & 0x000000ff) << 24);

    cout << n;

    return 0;
}
