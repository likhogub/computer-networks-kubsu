#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

using namespace std;

int PACKAGE_SIZE = 32;


char LOCALHOST[]  = "127.0.0.1";

typedef sockaddr_in SocketProps;
typedef int Socket;

Socket createSocketTCP() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("TCP socket creating error");
        exit(-1);
    }
    return sock;
}

Socket createSocketUDP() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("UDP socket creating error");
        exit(-1);
    }
    return sock;
}

SocketProps* createSocketProps(char* address, int port) {
    SocketProps* socketProps = new SocketProps;
    memset(socketProps, 0, sizeof(*socketProps));
    socketProps->sin_family = AF_INET;
    socketProps->sin_port = htons(port);
    socketProps->sin_addr.s_addr = inet_addr(address);
    return socketProps;
}

SocketProps* createSocketProps(int port) {
    return createSocketProps(LOCALHOST, port);
}

SocketProps* createSocketProps() {
    SocketProps* socketProps = new SocketProps;
    return socketProps;
}

Socket listenSocket(Socket sock, int queueLength=-1) {
    if (-1 == listen(sock, queueLength)) {
        perror("Socket listen error");
        exit(-1);
    }
    return 0;
}

Socket bindSocket(Socket sock, SocketProps* socketProps) {
    if (-1 == bind(sock, (sockaddr* )socketProps, sizeof(*socketProps))) {
        perror("Socket bind error");
        exit(-1);
    }
    return sock;
}

Socket connectSocket(Socket sock, SocketProps* socketProps) {
    if (-1 == connect(sock, (sockaddr *)socketProps, sizeof(*socketProps))) {
        perror("Socket connection error");
        exit(-1);
    }
    return sock;
}

Socket acceptSocket(Socket sock, SocketProps *socketProps) {
    socklen_t socketPropsSize = sizeof(*socketProps);
    Socket acceptedSocket = accept(sock, (sockaddr*)socketProps, &socketPropsSize);
    if (acceptedSocket == -1) {
        perror("Socket accept error");
        exit(-1);
    }
    return acceptedSocket;
}

Socket acceptSocket(Socket sock) {
    SocketProps* socketProps = new SocketProps;
    return acceptSocket(sock, socketProps);
}

int sendDataTCP(Socket sock, char* buffer, int bytes=PACKAGE_SIZE) {
    int len = send(sock, buffer, bytes, 0);
    if (len == -1) {
        perror("Sending error");
        exit(-1);
    }
    return len;
}

int sendDataUDP(Socket sock, SocketProps* socketProps, char* buf, int bytes=PACKAGE_SIZE) {
    return sendto(sock, buf, sizeof(buf), 0, (sockaddr*)socketProps, sizeof(*socketProps));
}

char* receiveDataTCP(Socket sock, int bytes=PACKAGE_SIZE) {
    char* buffer = new char[bytes];
    memset(buffer, 0, sizeof(buffer));
    int tmp = recv(sock, buffer, bytes, 0);
    if (tmp == -1) {
        perror("TCP Receiving error");
        exit(-1);
    }
    return buffer;
}

char* receiveDataUDP(Socket sock, SocketProps* socketProps, int bytes=PACKAGE_SIZE) {
    char* buffer = new char[bytes];
    memset(buffer, 0, sizeof(buffer));
    socklen_t socketPropsSize = sizeof(*socketProps);
    int tmp = recvfrom(sock, buffer, bytes, 0, (sockaddr*)socketProps, &socketPropsSize);
    if (tmp == -1) {
        perror("UDP Receiving error");
        exit(-1);
    }
    return buffer;
}

char* receiveDataUDP(Socket sock, int bytes=PACKAGE_SIZE) {
    SocketProps* socketProps = createSocketProps();
    return receiveDataUDP(sock, socketProps, bytes);
}
