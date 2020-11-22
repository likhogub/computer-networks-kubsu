#include "../EasySocket.h"
#include <string>
#include <sstream>
#include <iostream>
#include <netdb.h>
using namespace std;

string getRequestHeader(string address, int port) {
    stringstream requestHeader;
    requestHeader << "GET / HTTP/1.0\r\nHost: " << address << ":" << port << "\r\n\r\n";
    return requestHeader.str();
}

int main(int argc, char** args) {
    string address = "localhost";
    int port = 80;
    if (argc == 3) {
        address = args[1];
        port = atoi(args[2]);
    }


    hostent* host = gethostbyname(address.c_str());
    in_addr addr = *(in_addr*)host->h_addr_list[0];
    string ip_address = inet_ntoa(addr);
    

    Socket serverSock = createSocketTCP();
    SocketProps *serverProps = createSocketProps(ip_address.c_str(), port);
    connectSocket(serverSock, serverProps);

    string request = getRequestHeader(address, port);
    sendDataTCP(serverSock, request.c_str(), request.length());

    char buffer[100000] = {0};
    receiveDataTCP(serverSock, buffer, 100000);

    cout << buffer << endl;

    return 0;
}