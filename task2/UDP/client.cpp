#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "../EasySocket.h"
using namespace std;

const int MAX_DIAGNOSIS_LENGTH = 25;

struct request {
    double weight;
    double height;
};

struct response{
    double bmi;
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
};

void show_request(request* req) {
    cout << "Weight: " << req->weight << endl;
    cout << "Height: " << req->height << endl;
}

int main() {
    Socket sock = createSocketUDP();
    SocketProps* socketProps = createSocketProps(1234);
    
    
    char buf[] = "KEK";
    sendDataUDP(sock, socketProps, buf, sizeof(buf));

    return 0;
}