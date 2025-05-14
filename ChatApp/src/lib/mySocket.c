#include "mySocket.h"
#include "myLog.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

int setup_listener_socket(int port) {
    int sockfd;
    struct sockaddr_in addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        log_error("Failed to create socket");
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        log_error("Bind failed");
        return -1;
    }
    if (listen(sockfd, 10) < 0) {
        log_error("Listen failed");
        return -1;
    }
    return sockfd;
}

char* get_my_ip() {
    static char ip[INET_ADDRSTRLEN];
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in fake;
    fake.sin_family = AF_INET;
    fake.sin_port = htons(80);
    inet_pton(AF_INET, "8.8.8.8", &fake.sin_addr);
    connect(sock, (struct sockaddr*)&fake, sizeof(fake));
    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    getsockname(sock, (struct sockaddr*)&local, &len);
    inet_ntop(AF_INET, &local.sin_addr, ip, sizeof(ip));
    close(sock);
    return ip;
}

int create_connection(const char* ip, int port) {
    int sockfd;
    struct sockaddr_in addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return -1;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    return sockfd;
}
