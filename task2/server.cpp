#include "EasySocket.h"

char buf[] = "hello world2";

int main() {
    int sock = createSocket();
    bindSocket(sock, 1234);
    listenSocket(sock);
    int conn = acceptSocket(sock);
    while (true) {
        sendData(conn, receiveData(conn));
    }
}