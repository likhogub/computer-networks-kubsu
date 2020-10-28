#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "../EasySocket.h"
using namespace std;

const int MAX_DIAGNOSIS_LENGTH = 25;


char underweight[] = "underweight";
char overweight[] = "overweight";
char normal[] = "normal";


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
    cout << bindSocket(sock, socketProps) << endl;

    char buf[20] = {0};

    cout << receiveDataUDP(sock, buf, sizeof(buf))<< endl;
    

    return 0;
}