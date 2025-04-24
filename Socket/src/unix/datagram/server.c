#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_PATH "./server_socket"

int main() {
    int server_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len;

    unlink(SERVER_PATH);  

    server_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        printf("socket failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("bind failed");
        exit(1);
    }

    char buffer[1024];
    client_addr_len = sizeof(client_addr);
    int bytes_received = recvfrom(server_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (bytes_received == -1) {
        printf("recvfrom failed");
        exit(1);
    }
    buffer[bytes_received] = '\0';
    printf("Received from client: %s", buffer);

    const char *msg = "Hello from server by UNIX datagram\n";
    if (sendto(server_fd, msg, strlen(msg), 0, (struct sockaddr *)&client_addr, client_addr_len) == -1) {
        printf("sendto failed");
    } else {
        printf("Sent to client: %s", msg);
    }

    close(server_fd);
    unlink(SERVER_PATH);
    return 0;
}
