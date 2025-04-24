#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_PATH "./server_socket"
#define CLIENT_PATH "./client_socket"

int main() {
    int client_fd;
    struct sockaddr_un server_addr, client_addr;

    unlink(CLIENT_PATH);  

    client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        printf("socket failed");
        exit(1);
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path) - 1);

    if (bind(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        printf("bind failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    const char *msg = "Hello from client by UNIX datagram\n";
    if (sendto(client_fd, msg, strlen(msg), 0,(struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("sendto failed");
        exit(1);
    }
    printf("Sent to server: %s", msg);

    char buffer[1024];
    int bytes_received = recvfrom(client_fd, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
    if (bytes_received == -1) {
        printf("recvfrom failed");
        exit(1);
    }

    buffer[bytes_received] = '\0';
    printf("Received from server: %s", buffer);

    close(client_fd);
    unlink(CLIENT_PATH);
    return 0;
}
