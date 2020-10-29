#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "../EasySocket.h"
using namespace std;

const int MAX_DIAGNOSIS_LENGTH = 25;

Socket sock;
SocketProps* clientProps = new SocketProps;


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

double calcBMI(double h, double w) {
    h /= 100;
    return w/(h*h);
}

char* analyzeBMI(double bmi) {
    if (bmi < 18.6) return underweight;
    if (bmi > 25) return overweight;
    return normal;
}

response* create_response(request* req) {
    response* resp = new response;
    resp->bmi = calcBMI(req->height, req->weight);
    strcpy(resp->diagnosis, analyzeBMI(resp->bmi));
    return resp;
}

int send_response(response* resp) {
    return sendDataUDP(sock, clientProps, (char*)resp, sizeof(response));
}

int get_request(request* req) {
    return receiveDataUDP(sock, clientProps, (char*)req, sizeof(request));
}

void show_request(request* req) {
    cout << "Weight: " << req->weight << endl;
    cout << "Height: " << req->height << endl;
}

int handle_request() {
    request* req = new request;
    int tmp = get_request(req);
    if (tmp == 0) return -1;
    show_request(req);
    response* resp = create_response(req);
    return send_response(resp);
}

int main() {

    sock = createSocketUDP();
    SocketProps* serverProps = createSocketProps(1234);
    bindSocket(sock, serverProps);
    while (1) {
        handle_request();
    }


    return 0;
}