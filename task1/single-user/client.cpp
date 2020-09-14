#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

const char REQUESTS[] = "requests";
const char RESPONSES[] = "responses";
const int MAX_DIAGNOSIS_LENGTH = 25;

struct request {
    double weight;
    double height;
};

struct response{
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
    ifstream file(RESPONSES, ios::binary | ios::ate);
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

response* get_response(int start_from) {
    response* resp = new response;
    ifstream file(RESPONSES, ios::binary);
    file.seekg(start_from);
    file.read((char*)resp, sizeof(response));
    return resp;
}

void show_response(response* resp) {
    cout << "Your BodyMassIndex is " << resp->bmi << endl;
    cout << "It is "; 
    println(resp->diagnosis);
}

request* create_request(int weight, int height) {
    request* req = new request;
    req->weight = weight;
    req->height = height;
    return req;
}

int handle_response(int last_size) {
    response* resp = get_response(last_size);
    show_response(resp);
    return sizeof(response);
}

int main(){
    while (true) {
        double height;
        double weight;
        cout << "Enter your height (cm): ";
        cin >> height;
        cout << "Enter your weight (kg): ";
        cin >> weight;
        int last_size = get_file_size();
        send_request(create_request(weight, height));
        cout << "Awaiting..." << endl;
        while (true) {
            if (last_size < get_file_size()) {
                handle_response(last_size);
                last_size = get_file_size();
                break;
            }
        }
    }
    return 0;
}
