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
    ifstream file("responses", ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    return size;
}

int send_request(request student) {
    ofstream file("requests", ios::binary | ios::app);
    file.write ((char*)&student, sizeof (request));
    file.close();
    return 0;
}

string analyzeBMI(double bmi) {
    if (bmi < 18.6) return "underweight";
    if (bmi > 25) return "overweight";
    return "normal";
}

int show_response(response response_struct) {
    cout << "Your BodyMassIndex is " << response_struct.bmi << endl;
    cout << "It is " << analyzeBMI(response_struct.bmi) << endl;
    return 0;
}

request create_request(int weight, int height) {
    request req;
    req.weight = weight;
    req.height = height;
    return req;
}

response get_response(int read_from) {
    response resp;
    ifstream file("responses", ios::binary);
    file.seekg(read_from);
    file.read((char*)&resp, sizeof(response));
    return resp;
}


int main(){
    int height = 181;
    int weight = 85;

    send_request(create_request(weight, height));

    return 0;
}
