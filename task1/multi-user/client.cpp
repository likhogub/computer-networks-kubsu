#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

const char REQUESTS[] = "requests";
const char RESPONSES[] = "responses";
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

int get_file_size(char* name) {
    ifstream file(name, ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    if (size == -1) return 0;
    return size;
}

void send_request(request *student) {
    ofstream file(REQUESTS, ios::binary | ios::app);
    file.write ((char*)student, sizeof (request));
    file.close();
}

response* get_response(char* name, int start_from) {
    response* resp = new response;
    ifstream file(name, ios::binary);
    file.seekg(start_from);
    file.read((char*)resp, sizeof(response));
    return resp;
}

void show_response(response* resp) {
    cout << "Your BodyMassIndex is " << resp->bmi << endl;
    cout << "It is ";
    println(resp->diagnosis);
}

request* create_request(char* name, int weight, int height) {
    request* req = new request;
    strcpy(req->name, name);
    req->weight = weight;
    req->height = height;
    return req;
}

void handle_response(char* name, int last_size) {
    response* resp = get_response(name, last_size);
    show_response(resp);
}

int main(){
    while (true) {
        double height;
        double weight;
        char name[MAX_NAME_LENGTH];
        cout << "Enter your name: ";
        cin >> name;
        cout << "Enter your height (cm): ";
        cin >> height;
        cout << "Enter your weight (kg): ";
        cin >> weight;

        int last_size = get_file_size(name);
        send_request(create_request(name, weight, height));
        cout << "Awaiting..." << endl;
        while (true) {
            if (last_size < get_file_size(name)) {
                handle_response(name, last_size);
                last_size = get_file_size(name);
                break;
            }
        }
    }
    return 0;
}