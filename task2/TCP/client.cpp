#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "../../EasySocket.h"
using namespace std;

const int MAX_DIAGNOSIS_LENGTH = 25;

Socket sock;

struct request {
    double weight;
    double height;
};

struct response{
    double bmi;
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
};

int send_request(request *req) {
    return sendDataTCP(sock, (char*)req, sizeof(request));
}

int get_response(response *resp) {
    int tmp = receiveDataTCP(sock, (char*)resp, sizeof(response));
    return tmp;
}

void show_response(response* resp) {
    cout << "Your BodyMassIndex is " << resp->bmi << endl;
    cout << "It is " << resp->diagnosis << endl;
}

int handle_response() {
    response* resp = new response;
    int tmp = get_response(resp);
    if (tmp == 0) return -1;
    show_response(resp);
    return tmp;
}

int main() {
    while (1) {
        sock = createSocketTCP();

        char address[20] = {0};
        cout << "Enter server address:" << endl;
        cin >> address;
        
        char port[10] = {0};
        cout << "Enter server port:" << endl;
        cin >> port;

        SocketProps* props = createSocketProps(address, atoi(port));

        if (connectSocket(sock, props) == -1) {
            perror("Connection failed");
            continue;
        }

        while (1) {
            request req;
            cout << "Enter your height (cm): ";
            cin >> req.height;
            cout << "Enter your weight (kg): ";
            cin >> req.weight;
            if (send_request(&req) == -1 || handle_response() == -1) {
                cout << "Connection lost" << endl;
                close(sock);
                break;
            }
        }
    }
    return 0;
}