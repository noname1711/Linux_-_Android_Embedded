#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_PATH "./my_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    socklen_t addr_len;

    unlink(SOCKET_PATH);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        printf("socket failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("bind failed");
        exit(1);
    }

    if (listen(server_fd, 5) == -1) {
        printf("listen failed");
        exit(1);
    }

    addr_len = sizeof(server_addr); 
    client_fd = accept(server_fd, (struct sockaddr *)&server_addr, &addr_len);
    if (client_fd == -1) {
        printf("accept failed");
        exit(1);
    }

    char buffer[1024];
    int bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_received == -1) {
        printf("read failed");
        exit(1);
    }
    buffer[bytes_received] = '\0';
    printf("Received from client: %s", buffer);

    const char *msg = "Hello from server by UNIX stream\n";
    if (write(client_fd, msg, strlen(msg)) == -1) {
        printf("write failed");
    } else {
        printf("Sent to client: %s", msg);
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
