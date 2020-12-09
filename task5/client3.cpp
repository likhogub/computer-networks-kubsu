#include <iostream>
#include <mutex>
#include <pthread.h>
#include "../EasySocket.h"
#define MSGSIZE 256
#define PKGSIZE 512
using namespace std;

struct Package {
    char from[25] = {0};
    char to[25] = {0};
    char msg[MSGSIZE] = {0};
    Package(string from, string to, string msg) {
        strncpy(this->from, from.c_str(), 25);
        strncpy(this->to, to.c_str(), 25);
        strncpy(this->msg, msg.c_str(), MSGSIZE);
    }
    Package(string from, string msg) {
        Package(from, "ALL", msg);
    }
};

int sendPackageToSocket(Socket sock, Package package) {
    return sendDataTCP(sock, (char*)&package, PKGSIZE);
}

void* serverListener(void* sockPtr) {
    Socket sock = *(Socket*)sockPtr;
    while (true) {
        char buffer[PKGSIZE] = {0};
        if (0 >= receiveDataTCP(sock, buffer, PKGSIZE)) {
            cout << "Connection aborted" << endl;
            close(sock);
            return 0;
        } else {
            Package package = *(Package*)buffer;
            cout << package.from << " " << package.to << " " << package.msg << endl;
        }
    }
    return 0;
}

int main(int argc, char** args) {
    int port = 7777;
    if (argc == 2) {
        port = atoi(args[1]);
    }
    
    Socket serverSocket = createSocketTCP();
    SocketProps* serverProps = createSocketProps(port);
    connectSocket(serverSocket, serverProps);
    pthread_t ttt;
    pthread_create(&ttt, NULL, serverListener, (void*)&serverSocket);
    string from = "olli";

    Package package = *(new Package(from, "kek"));
    sendPackageToSocket(serverSocket, package);
    while (true) {
        string msg;
        cin >> msg;
        Package package(from, msg);
        sendPackageToSocket(serverSocket, package);
    }


    return 0;
}