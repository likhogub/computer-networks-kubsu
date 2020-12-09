#include <map>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include "../EasySocket.h"
#define PKGSIZE 1024
#define MSGSIZE 256
using namespace std;


struct Package {
    char sender[25] = {0};
    char reciever[25] = {0};
    char msg[MSGSIZE] = {0};
};

Package createPackage(string sender, string reciever, string msg) {
    Package package;
    strncpy(package.sender, sender.c_str(), 25);
    strncpy(package.reciever, reciever.c_str(), 25);
    strncpy(package.msg, msg.c_str(), MSGSIZE);
    return package;
}

Package createPackage(string sender, string msg) {
    return createPackage(sender, "ALL", msg);
}

void sendPackageToSocket(Package& package, Socket sock) {
    sendDataTCP(sock, (char*)&package, PKGSIZE);
}

Package recvPackageFromSocket(Socket sock) {
    char buffer[PKGSIZE] = {0}; 
    receiveDataTCP(sock, (char*)&buffer, PKGSIZE);
    return *(Package*)buffer;
}

int main(int argc, char** args) {
    int port = 7777;
    if (argc == 2) {
        port = atoi(args[1]);
    }
    
    Socket serverSocket = createSocketTCP();
    SocketProps* serverProps = createSocketProps(port);
    connectSocket(serverSocket, serverProps);

    Package package = createPackage("hi", "lol", "kek");
    sendPackageToSocket(package, serverSocket);

    return 0;
}