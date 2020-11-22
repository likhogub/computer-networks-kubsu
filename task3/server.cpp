#include <iostream>
#include <string>
#include <sstream>
#include "../EasySocket.h"
using namespace std;

int main(int argc, const char** args) {
    int port = 1234;
    if (argc > 1) {
        port = atoi(args[1]);
    }
    Socket serverSock = createSocketTCP();
    SocketProps* serverSockProps = createSocketProps(port);
    bindSocket(serverSock, serverSockProps);
    listenSocket(serverSock);
    cout << "Server started at localhost:" << port << endl;
    while (1) {
        cout << "Waiting for connection..." << endl;
        Socket clientSock = acceptSocket(serverSock);
        cout << "Connection opened" << endl;
        char buffer[10000] = {0};
        receiveDataTCP(clientSock, buffer, 10000);
        stringstream response_body;
        stringstream response;
        response_body << ""
            << "<title>Test C++ HTTP Server</title>\n"
            << "<h1>Test page</h1>\n"
            << "<p>This is body of the test page...</p>\n"
            << "<h2>Request headers</h2>\n"
            << "<pre>" << buffer << "</pre>\n"
            << "<em><small>Test C++ Http Server</small></em>\n";
        response << "" 
            << "HTTP/1.1 200 OK\r\n"
            << "Version: HTTP/1.1\r\n"
            << "Content-Type: text/html; charset=utf-8\r\n"
            << "Content-Length: " 
            << response_body.str().length()
            << "\r\n\r\n"
            << response_body.str();
        sendDataTCP(clientSock, response.str().c_str(), response.str().length());
        close(clientSock);
        cout << "Connection closed" << endl;
    }
    return 0;
}