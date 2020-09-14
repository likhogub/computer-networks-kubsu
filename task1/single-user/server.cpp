#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

const char REQUESTS[] = "requests";
const char RESPONSES[] = "responses";

struct request {
    int weight;
    int height;
};

struct response{
    double bmi;
};

int get_file_size() {
    ifstream file(REQUESTS, ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    return size;
}

void send_response(response* response_struct) {
    ofstream file(RESPONSES, ios::binary | ios::app);
    file.write ((char*)response_struct, sizeof(response));
    file.close();
}

double calcBMI(request* req) {
    int height = req->height;
    int weight = req->weight;
    double h = ((double)height)/100;
    double w = (double)weight;
    return w/(h*h);
}

response* create_response(double bmi) {
    response* resp = new response;
    resp->bmi = bmi;
    return resp;
}

void show_request(request* resp) {
    cout << "Weight: " << resp->weight << endl;
    cout << "Height: " << resp->height << endl;
}

request* get_request(int start_from) {
    request* req = new request;
    ifstream file(REQUESTS, ios::binary);
    file.seekg(start_from);
    file.read((char*)req, sizeof(request));
    return req;
}

int handle_request(int last_size) {
    request* new_req = get_request(last_size);
    show_request(new_req);
    double bmi = calcBMI(new_req);
    response* resp = create_response(bmi);
    send_response(resp);
    return sizeof(request);
}

int main() {
    cout << "Server started..." << endl;
    int last_size = get_file_size();
    while (true) {
        if (last_size < get_file_size()) 
        last_size += handle_request(last_size);
    }
    return 0;
}
