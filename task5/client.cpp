#include "../EasySocket.h"
#include <pthread.h>
#include <iostream>
#define MSGSIZE 256
using namespace std;

void* serverListenerThread(void* _serverSocket) {
    Socket serverSocket = *(Socket*)_serverSocket;
    while (true) {
        char buffer[MSGSIZE] = {0};
        if (0 >= receiveDataTCP(serverSocket, buffer, MSGSIZE)) {
            cout << "Connection lost" << endl;
            break;
        } else { 
            cout << "> " << buffer << endl;
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
    pthread_t* ttt;
    pthread_create(ttt, NULL, serverListenerThread, (void*)&serverSocket);

    while (true) {
        char buffer[MSGSIZE] = {0};
        cin >> buffer;
        sendDataTCP(serverSocket, buffer, MSGSIZE);
    }

    return 0;
}