#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_PATH "./my_socket"

int main() {
    int client_fd;
    struct sockaddr_un server_addr;

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        printf("socket failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("connect failed");
        exit(1);
    }

    const char *msg = "Hello from client by UNIX stream\n";
    if (write(client_fd, msg, strlen(msg)) == -1) {
        printf("write failed");
        exit(1);
    }
    printf("Sent to server: %s", msg);

    char buffer[1024];
    int bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_received == -1) {
        printf("read failed");
        exit(1);
    }
    buffer[bytes_received] = '\0';
    printf("Received from server: %s", buffer);

    close(client_fd);
    return 0;
}
