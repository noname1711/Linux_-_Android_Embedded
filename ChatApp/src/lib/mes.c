#include "mes.h"
#include "connection.h"
#include "myLog.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void send_message(int connection_id, const char* message) {
    int socket_fd = get_socket_by_id(connection_id);
    if (socket_fd < 0) {
        log_error("Invalid connection ID");
        return;
    }
    send(socket_fd, message, strlen(message), 0);
    log_info("Message sent to ID %d.", connection_id);
}

void receive_message(int sender_fd) {
    char buffer[1024];
    int len = recv(sender_fd, buffer, sizeof(buffer) - 1, 0);

    if (len <= 0) {
        if (len == 0) {
            log_info("Peer disconnected (socket %d)", sender_fd);
        } else {
            log_error("recv() error on socket %d", sender_fd);
        }
        return;
    }

    buffer[len] = '\0';
    log_info("Message from ID %d: %s", get_id_by_socket(sender_fd), buffer);
}

