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
        printf("No port number and server number\n");
        exit(1);
    }else{
        port_no = atoi(argv[1]);
        if(port_no < 1024 || port_no > 65535) {
            printf("Port number must be between 1024 and 65535\n");
            exit(1);
        }
    }

    int client_fd;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        printf("socket failed");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);
    inet_pton(AF_INET, argv[2], &server_addr.sin_addr);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("connect failed");
        close(client_fd);
        exit(1);
    }

    const char *msg = "Hello from client by IPv4 stream\n";
    if (write(client_fd, msg, strlen(msg)) == -1) {
        printf("write failed");
        close(client_fd);
        exit(1);
    }
    printf("Sent to server: %s", msg);

    char buffer[1024];
    int bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the string
        printf("Received from server: %s", buffer);
    } else {
        printf("read failed");
    }
    close(client_fd);
    return 0;
}