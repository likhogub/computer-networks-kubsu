#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

#define PACKAGE_SIZE 32

/*Receives and returns data from fd*/
char* receiveData(int fd, int bytes=PACKAGE_SIZE) {
    char* buffer = new char[bytes];
    memset(buffer, 0, sizeof(buffer));
    int c = 0;
    int tmp = 0;
    while (c < bytes) {
        tmp = recv(fd, buffer + c, 1, 0);
        if (tmp == -1) {
            perror("Receiving error");
            exit(-1);
        }
        c += tmp;
        if (tmp == 0) break;
    }
    return buffer;
}

/*Sends data from buffer to fd*/
int sendData(int fd, char* buffer, int bytes=PACKAGE_SIZE) {
    int len = send(fd, buffer, bytes, 0);
    if (len == -1) {
        perror("Sending error");
        exit(-1);
    }
    return len;
}

/*Returns new socket fd*/
int createSocket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creating error");
        exit(-1);
    }
    return sock;
}

/*Binds socket to "localhost:PORT:*/
int bindSocket(int sock, int port) {
    struct sockaddr_in sock_prop;
    sock_prop.sin_family = AF_INET;
    sock_prop.sin_port = htons(port);
    sock_prop.sin_addr.s_addr = htonl(INADDR_ANY);
    if (-1 == bind(sock, (struct sockaddr* )&sock_prop, sizeof(sock_prop))) {
        perror("Bind error");
        exit(-1);
    }
    return 0;
}

/*Toggles listening for fd*/
int listenSocket(int sock, int queue=-1) {
    if (-1 == listen(sock, queue)) {
        perror("Listen error");
        exit(-1);
    }
    return 0;
}

/*Connects socket to "ADDRESS:PORT"*/
int connectSocket(int sock, char* address, int port) {
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  
    servaddr.sin_addr.s_addr = inet_addr(address);  
    if (-1 == connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        perror("Connection error");
        exit(-1);
    }
    return 0;
}

/*Accepts new connection and returns fd*/
int acceptSocket(int sock) {
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    int new_conn_fd = accept(sock, (struct sockaddr*)&client_addr, &length);
    if (new_conn_fd == -1) {
        perror("Accept error");
        exit(-1);
    }
    return new_conn_fd;
}