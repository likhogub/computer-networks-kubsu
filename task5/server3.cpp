#include "../EasySocket.h"
#include <iostream>
#include <mutex>
#include <set>
#include <map>
#include <pthread.h>
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

map<string, Socket> nickToSocketMap;
mutex nickToSocketMutex;


int sendPackageToSocket(Socket sock, Package package) {
    return sendDataTCP(sock, (char*)&package, PKGSIZE);
}

void sendPackage(Package package) {
    for (auto client : nickToSocketMap) {
        if (client.first.compare(package.from) == 0) {
            continue;
        } else {
            sendPackageToSocket(client.second, package);
        }
    }
}

void* clientThread(void* sockPtr) {
    Socket sock = *(Socket*)sockPtr;
    string nick;
    do {
        char buffer[PKGSIZE] = {0};
        if (0 >= receiveDataTCP(sock, buffer, PKGSIZE)) {
            cout << "Auth aborted" << endl;
            close(sock);
            return 0;
        }
        Package package = *(Package*)buffer;
        nick = package.from;
        cout << nick << endl;
    } while (nickToSocketMap.count(nick) > 0);
    sendPackageToSocket(sock, *(new Package("SERVER", "Access granted")));
    while (true) {
        char buffer[PKGSIZE] = {0};
        if (0 >= receiveDataTCP(sock, buffer, PKGSIZE)) {
            cout << "Connection aborted" << endl;
            close(sock);
            return 0;
        }
        Package package = *(Package*)buffer;
        cout << package.from << " " << package.to << " " << package.msg << endl;
    }

}

int main(int argc, char** args) {
    int port = 7777;
    if (argc == 2) {
        port = atoi(args[1]);
    }
    Socket serverSocket = createSocketTCP();
    SocketProps* serverProps = createSocketProps(port);
    bindSocket(serverSocket, serverProps);
    listenSocket(serverSocket);

    while(true) {
        Socket clientSock = acceptSocket(serverSocket);
        cout << clientSock << endl;
        pthread_t ttt;
        pthread_create(&ttt, NULL, clientThread, (void*)&clientSock);
    }


    return 0;
}

