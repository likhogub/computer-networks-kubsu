#include "../EasySocket.h"
#include <iostream>
#include <mutex>
#include <set>
#include <pthread.h>
#define MSGSIZE 256
using namespace std;

mutex setMutex;
mutex clientCounterMutex;
int clientCounter;
set <int> clientSocketSet;

int clientNotify(Socket excludedSocket, char* msg);

void* clientListenerThread(void* _clientSocket) {
    Socket clientSocket = *(Socket*)_clientSocket;
    cout << "Client connected " << clientSocket << endl; 
    while (true) {
        char buffer[MSGSIZE] = {0};
        if (0 >= receiveDataTCP(clientSocket, buffer, MSGSIZE))
            break;
        else
            clientNotify(clientSocket, buffer);
    }
    setMutex.lock();
    cout << "Client disconnected " << clientSocket << endl;
    close(clientSocket);
    clientSocketSet.erase(clientSocket);
    setMutex.unlock();
    clientCounterMutex.lock();
    clientCounter--;
    clientCounterMutex.unlock();
    return 0;
}

int clientNotify(Socket excludedSocket, char* msg) {
    for (auto clientSocket : clientSocketSet) {
        if (clientSocket == excludedSocket)
            continue;
        sendDataTCP(clientSocket, msg, MSGSIZE);
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
    bindSocket(serverSocket, serverProps);
    listenSocket(serverSocket);
    while (true) {
        Socket clientSocket = acceptSocket(serverSocket);
        clientCounterMutex.lock();
        clientCounter++;
        clientCounterMutex.unlock();
        setMutex.lock();
        clientSocketSet.insert(clientSocket);
        setMutex.unlock();
        pthread_t* ttt;
        pthread_create(ttt, NULL, clientListenerThread, (void*)&clientSocket);
    }    
    return 0;
}