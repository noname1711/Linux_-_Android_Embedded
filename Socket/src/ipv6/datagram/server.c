#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]) {
    int port_no;
    if(argc < 2) {
        printf("No port number provided\n");
        exit(1);
    }else{
        port_no = atoi(argv[1]);
        if(port_no < 1024 || port_no > 65535) {
            printf("Port number must be between 1024 and 65535\n");
            exit(1);
        }
    }

    int server_fd;
    struct sockaddr_in6 server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_fd < 0) {
        printf("Socket creation failed");
        exit(1);
    }
    server_addr.sin6_family = AF_INET;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(port_no);
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Bind failed");
        exit(1);
    }

    int n = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
    if(n < 0) {
        printf("Receive failed");
        exit(1);
    }
    buffer[n] = '\0';
    printf("Received from client: %s\n", buffer);

    char *msg="Hello from server by IPv6 datagram\n";
    int len = strlen(msg);
    if(sendto(server_fd, msg, len, 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
        printf("Send failed");
        exit(1);
    }
    printf("Sent to client: %s\n", msg);
    close(server_fd);
    return 0;
}