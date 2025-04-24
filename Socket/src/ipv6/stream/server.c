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

    int server_fd, client_fd;
    struct sockaddr_in6 server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    socklen_t client_addr_len = sizeof(client_addr);
    server_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if(server_fd < 0) {
        perror("socket");
        exit(1);
    }
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(port_no);

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("bind failed");
        exit(1);
    }
    if(listen(server_fd, 5) < 0) {
        printf("listen failed");
        exit(1);
    }
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);    
    if(client_fd < 0) {
        printf("accept failed");
        exit(1);
    }

    char buffer[1024];
    int bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; 
        printf("Received from client: %s", buffer);
    } else {
        printf("read failed");
    }

    char *msg = "Hello from server by IPv6 stream\n";
    write(client_fd, msg, strlen(msg));
    printf("Sent to client: %s\n", msg);
    close(client_fd);
    close(server_fd);
    return 0;
}