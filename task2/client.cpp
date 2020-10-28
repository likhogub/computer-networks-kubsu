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
    connectSocket(sock, props);

    stud s;
    s.first = 5;
    s.second = 77;

    sendDataTCP(sock, (char*)&s);

    return 0;
}