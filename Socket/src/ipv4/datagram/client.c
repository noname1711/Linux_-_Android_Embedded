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
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    socklen_t client_len = sizeof(client_addr);
    
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        printf("socket failed");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);

    inet_pton(AF_INET, argv[2], &server_addr.sin_addr);

    char *msg= "Hello from client by IPv4 datagram\n";
    if (sendto(client_fd, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("sendto failed");
        exit(1);
    }
    printf("Sent to server: %s\n", msg);

    char buffer[1024];
    int n = recvfrom(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
    if (n < 0) {
        printf("recvfrom failed");
        exit(1);
    }
    buffer[n] = '\0';
    printf("Received from server: %s\n", buffer);
    close(client_fd);
    return 0;
} 