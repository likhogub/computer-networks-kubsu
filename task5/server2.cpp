#include <map>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include "../EasySocket.h"
#define PKGSIZE 1024
#define MSGSIZE 256
using namespace std;

mutex dbMutex;

class DB {
    map <string, Socket> stringToSocketMap;
    map <Socket, string> socketToStringMap;
        
    public:

        Socket& operator[] (string nick) {
            return this->stringToSocketMap[nick];
        } 

        string& operator[] (Socket sock) {
            return this->socketToStringMap[sock];
        }

        map <Socket, string>& getDB() {
            return socketToStringMap;
        }

        void insert(Socket sock, string nick) {
            dbMutex.lock();
            (*this)[sock] = nick;
            (*this)[nick] = sock;
            dbMutex.unlock();
        }
        
        void remove(string nick) {
            dbMutex.lock();
            Socket sock = this->stringToSocketMap[nick];
            this->socketToStringMap.erase(sock);
            this->stringToSocketMap.erase(nick);
            dbMutex.unlock();
        }

        void remove(Socket sock) {
            dbMutex.lock();
            string nick = this->socketToStringMap[sock];
            this->stringToSocketMap.erase(nick);
            this->socketToStringMap.erase(sock);
            dbMutex.unlock();
        }

};

struct Package {
    char sender[25] = {0};
    char reciever[25] = {0};
    char msg[MSGSIZE] = {0};
};

DB db;

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

Package* recvPackageFromSocket(Socket sock) {
    char* buffer = new char[PKGSIZE];
    if (0 >= receiveDataTCP(sock, (char*)&buffer, PKGSIZE))
        return 0;
    else 
        return (Package*)buffer;
}

void* sendPackage(void* packagePtr) {
    Package package = *(Package*)packagePtr;
    cout << package.sender << endl;
    cout << package.reciever << endl;
    cout << package.msg << endl;

    if (strncmp(package.sender, "ALL", 4) == 0) {
        dbMutex.lock();
        Socket senderSocket = db[package.sender];
        for (auto client : db.getDB()) {
            if (client.first == senderSocket)
                continue;
            sendPackageToSocket(package, client.first);
        }
        dbMutex.unlock();
    } else {
        dbMutex.lock();
        Socket recieverSocket = db[package.reciever];
        dbMutex.unlock();
        sendPackageToSocket(package, recieverSocket);
    }
    return 0;
}

void disconnect(Socket sock) {
    db.remove(sock);
    cout << "Disconnected " << sock << endl;
    close(sock);
}

void asyncSendPackage(Package package) {
    pthread_t ttt;
    pthread_create(&ttt, NULL, sendPackage, (void*)&package);
}

void* clientThread(void* clientSocketPtr) {
    cout << "Async thread started" << endl;
    Socket clientSocket = *(Socket*)clientSocketPtr;
    Package* packagePtr = recvPackageFromSocket(clientSocket);
    if (packagePtr == 0) {
        disconnect(clientSocket);
        return 0;
    }
    Package package = *packagePtr;
    db.insert(clientSocket, package.sender);
    cout << "Authentified " << package.sender << endl;
    while (true) {
        Package* packagePtr = recvPackageFromSocket(clientSocket);
        if (packagePtr == 0) {
            break;
        }
        Package package = *packagePtr;
        asyncSendPackage(package);
    }
    disconnect(clientSocket);
    return 0;
}

void asyncClientThread(Socket clientSocket) {
    pthread_t ttt;
    pthread_create(&ttt, NULL, clientThread, (void*)&clientSocket);
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



    return 0;
}