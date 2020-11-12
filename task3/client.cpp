#include "../EasySocket.h"
#include <string>
#include <iostream>
#include <netdb.h>

using namespace std;

char request[] = "GET /json-ru.html HTTP/1.0\r\nHost: www.json.org\r\n\r\n";

int main() {
    hostent *host;
    host = gethostbyname("www.json.org");
    in_addr ad = *(in_addr*)host->h_addr_list[0];
    
    char* str_address = inet_ntoa(ad);
    cout << str_address << endl;

    Socket sock = createSocketTCP();
    SocketProps *props = createSocketProps(str_address, 80);

    connectSocket(sock, props);
    sendDataTCP(sock, request, sizeof(request));
    
    char lol[100000] = {0};
    int recvBytes = 0;
    int totalBytes = 0;
    do {
        recvBytes = receiveDataTCP(sock, lol+totalBytes, 1);
        totalBytes += recvBytes;
    } while (recvBytes != 0);

    cout << lol << endl;

    return 0;
}
