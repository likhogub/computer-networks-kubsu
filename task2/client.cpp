#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
using namespace std;

struct sockaddr_in serv_addr; 

char buffer[] = "Hello";

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(1234);

    bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sock, 0);

    int new_fd = accept(sock, (struct sockaddr *)&serv_addr, (socklen_t*)&serv_addr);
    write(new_fd, buffer, sizeof(buffer));

    


return 0;
}