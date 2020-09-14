#include <iostream>
#include <fstream>
#include <string>
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
    ifstream file(RESPONSES, ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    return size;
}

void send_request(request *student) {
    ofstream file(REQUESTS, ios::binary | ios::app);
    file.write ((char*)student, sizeof (request));
    file.close();
}

string analyzeBMI(double bmi) {
    if (bmi < 18.6) return "underweight";
    if (bmi > 25) return "overweight";
    return "normal";
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
    cout << "It is " << analyzeBMI(resp->bmi) << endl;
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
        int height;
        int weight;

        cout << "Enter your height (cm): ";
        cin >> height;
        cout << "Enter your weight (kg): ";
        cin >> weight;
        send_request(create_request(weight, height));

        int last_size = get_file_size();
        while (true) {
            if (last_size < get_file_size()) {
                last_size += handle_response(last_size);
                break;
            }
        }
    }
    return 0;
}
