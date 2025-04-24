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
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    char buffer[1024];
    socklen_t client_len = sizeof(client_addr);
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        printf("socket failed");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("bind failed");
        exit(1);
    }

    int n = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
    if (n < 0) {
        printf("recvfrom failed");
        exit(1);
    }
    buffer[n] = '\0';
    printf("Received from client: %s\n", buffer);

    sendto(server_fd, "Hello from server by IPv4 datagram\n", sizeof("Hello from server by IPv4 datagram\n"), 0, (struct sockaddr *)&client_addr, client_len);
    printf("Sent to client: Hello from server by IPv4 datagram\n");
    close(server_fd);
    return 0;
}