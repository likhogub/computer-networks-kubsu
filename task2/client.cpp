#include "EasySocket.h"
#include <iostream>
using namespace std;

char buf[] = "hello world";
char address[] = "127.0.0.1";

int main() {
    int sock = createSocket();
    connectSocket(sock, address, 1234);
    char buf[16];
    while (true) {
        cin >> buf;    
        sendData(sock, buf);
        cout << receiveData(sock) << endl;
    }
}