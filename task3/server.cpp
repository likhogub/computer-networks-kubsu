#include "../EasySocket.h"
#include <string>
#include <sstream>

#include <iostream>
using namespace std;



int main() {
    Socket sock = createSocketTCP();
    SocketProps *props = createSocketProps(1235);
    cout << bindSocket(sock, props) << endl;
    cout << listenSocket(sock) << endl;
    Socket clientSock = acceptSocket(sock);
    cout << clientSock << endl;
    char buf[10000] = {0};

    int recvBytes = 0;
    int totalBytes = 0;
    do {
        recvBytes = receiveDataTCP(clientSock, buf+totalBytes, 1);
        cout << *(buf+totalBytes);
        totalBytes += recvBytes;
    } while (totalBytes < 256 && recvBytes != 0);
    cout << "lol" << endl;
    stringstream response_body;
    response_body << "<title>Test C++ HTTP Server</title>\n"
        << "<h1>Test page</h1>\n"
        << "<p>This is body of the test page...</p>\n"
        << "<h2>Request headers</h2>\n"
        << "<pre>" << buf << "</pre>\n"
        << "<em><small>Test C++ Http Server</small></em>\n";

    stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
        << "Version: HTTP/1.1\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << response_body.str().length()
        << "\r\n\r\n"
        << response_body.str();

    char new_buf[10000];
    strncpy(new_buf, response.str().c_str(), response.str().length());
    sendDataTCP(clientSock, new_buf, response.str().length());
    cin >> new_buf;

    return 0;
}