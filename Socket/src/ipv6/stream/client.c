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
    struct sockaddr_in6 server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    client_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if(client_fd < 0) {
        printf("socket failed");
        exit(1);
    }

    server_addr.sin6_family = AF_INET6;  
    server_addr.sin6_port = htons(port_no);  
    inet_pton(AF_INET6, argv[2], &server_addr.sin6_addr);

    if(connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("connect failed");
        exit(1);
    }

    char *msg = "Hello from client by IPv6 stream\n";
    write(client_fd, msg, strlen(msg));
    printf("Sent to server: %s\n", msg);

    char buffer[1024];
    int bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; 
        printf("Received from server: %s", buffer);
    } else {
        printf("read failed");
    }

    close(client_fd);
    return 0;
}