#include <iostream>
#include "EasySocket.h"
using namespace std;

struct stud {
    int first;
    int second;
};

int main() {
    Socket sock = createSocketTCP();
    SocketProps* props = createSocketProps(4321);
    bindSocket(sock, props);
    listenSocket(sock);
    Socket conn = acceptSocket(sock);

    stud* w = (stud*)receiveDataTCP(conn);
    cout << w->first << endl;
    cout << w->second << endl;    

    return 0;
}