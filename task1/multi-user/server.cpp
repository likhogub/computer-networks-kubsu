#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
using namespace std;

const char REQUESTS[] = "requests";
const int MAX_NAME_LENGTH = 50;
const int MAX_DIAGNOSIS_LENGTH = 25;

struct request {
    char name[MAX_NAME_LENGTH];
    double weight;
    double height;
};

struct response{
    char name[MAX_NAME_LENGTH];
    double bmi;
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
};

void println(char* char_arr_ptr) {
    int i = 0;
    while (char_arr_ptr[i] != '\0') {
        cout << char_arr_ptr[i];
        i++;
    }
    cout << endl;
}

int get_file_size() {
    ifstream file(REQUESTS, ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    if (size == -1) return 0;
    return size;
}

void send_response(response* response_struct) {
    ofstream file(response_struct->name, ios::binary | ios::app);
    file.write ((char*)response_struct, sizeof(response));
    file.close();
}

double calcBMI(double h, double w) {
    h /= 100;
    return w/(h*h);
}

char* analyzeBMI(double bmi) {
    if (bmi < 18.6) return "underweight";
    if (bmi > 25) return "overweight";
    return "normal";
}

response* create_response(request* req) {
    response* resp = new response;
    resp->bmi = calcBMI(req->height, req->weight);
    strcpy(resp->name, req->name);
    strcpy(resp->diagnosis, analyzeBMI(resp->bmi));
    return resp;
}

void show_request(request* resp) {
    cout << "Weight: ";
    println(resp->name);
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

int handle_request(int start_from) {
    request* req = get_request(start_from);
    show_request(req);
    response* resp = create_response(req);
    send_response(resp);
    return sizeof(request);
}

int main() {
    cout << "Server started..." << endl;
    int start_from = get_file_size();
    while (true) {
        if (start_from < get_file_size()) 
            start_from += handle_request(start_from);
    }
    return 0;
}