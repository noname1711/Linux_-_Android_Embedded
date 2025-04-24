#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]) {
    int port_no;
    if(argc < 3) {
        printf("No port number and server provided\n");
        exit(1);
    }else{
        port_no = atoi(argv[1]);
        if(port_no < 1024 || port_no > 65535) {
            printf("Port number must be between 1024 and 65535\n");
            exit(1);
        }
    }

    int client_fd;
    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    client_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(client_fd < 0) {
        printf("Socket creation failed");
        exit(1);
    }

    server_addr.sin6_family = AF_INET;
    server_addr.sin6_port = htons(port_no);
    inet_pton(AF_INET6, argv[2], &server_addr.sin6_addr);

    char *msg="Hello from client by IPv6 datagram\n";
    int len = strlen(msg);
    if(sendto(client_fd, msg, len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Send failed");
        exit(1);
    }
    printf("Sent to server: %s\n", msg);
    
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    int n = recvfrom(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_len);
    if(n < 0) {
        printf("Receive failed");
        exit(1);
    }
    buffer[n] = '\0';
    printf("Received from server: %s\n", buffer);
    close(client_fd);
    return 0;
}