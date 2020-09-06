#include <iostream>
#include <fstream>
using namespace std;

struct request {
    int weight;
    int height;
};

struct response{
    double bmi;
};

int get_file_size() {
    ifstream file("requests", ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    return size;
}

int send_response(response response_struct) {
    ofstream file("responses", ios::binary | ios::app);
    file.write ((char*)&response_struct, sizeof(response));
    file.close();
    return 0;
}

double calcBMI(request request_struct) {
    return ((double)request_struct.weight)/
        (request_struct.height)/(request_struct.height)*10000;
}

response create_response(double bmi) {
    response resp;
    resp.bmi = bmi;
    return resp;
}

int show_request(request response_struct) {
    cout << "Weight: " << response_struct.weight << endl;
    cout << "Height: " << response_struct.height << endl;
    return 0;
}

request get_request(int read_from) {
    request req;
    ifstream file("requests", ios::binary);
    file.seekg(read_from);
    file.read((char*)&req, sizeof(request));
    return req;
}

int handle_request(int last_size) {
    request new_req = get_request(last_size);
    show_request(new_req);
    double bmi = calcBMI(new_req);
    response resp = create_response(bmi);
    send_response(resp);
    return last_size + sizeof(request);
}

int main() {
    //cout << get_file_size() << endl;
    handle_request(16);
    return 0;
}

