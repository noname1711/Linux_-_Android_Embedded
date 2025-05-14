#include "mes.h"
#include "connection.h"
#include "myLog.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_MESSAGE_LENGTH 100

void send_message(int connection_id, const char* message) {
    int socket_fd = get_socket_by_id(connection_id);
    if (socket_fd < 0) {
        log_error("Invalid connection ID");
        return;
    }
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getpeername(socket_fd, (struct sockaddr*)&addr, &addr_len) == -1) {
        perror("getpeername");
        log_error("Failed to get peer address for ID %d", connection_id);
        return;
    }
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, sizeof(ip));
    int listen_port = get_peer_listen_port(socket_fd);
    if (listen_port < 0) {
        log_error("Failed to find listening port for connection ID %d", connection_id);
        return;
    }
    if (strlen(message) > MAX_MESSAGE_LENGTH) {
        log_error("Message too long. Maximum allowed length is %d characters.", MAX_MESSAGE_LENGTH);
        return;
    }
    send(socket_fd, message, strlen(message), 0);
    log_info("Message sent to %s:%d", ip, listen_port);
}


RecvStatus receive_message(int sender_fd, char* output_buffer, int buffer_size) {
    int len = recv(sender_fd, output_buffer, buffer_size - 1, 0);
    if (len <= 0) {
        return (len == 0) ? RECV_DISCONNECT : RECV_DISCONNECT;
    }
    output_buffer[len] = '\0';
    if (strncmp(output_buffer, "EXIT", 4) == 0) {
        return RECV_EXIT;
    }
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getpeername(sender_fd, (struct sockaddr*)&addr, &addr_len) == -1) {
        perror("getpeername");
        log_error("Failed to get peer address for socket %d", sender_fd);
        return RECV_DISCONNECT;
    }
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, sizeof(ip));
    int listen_port = get_peer_listen_port(sender_fd);
    if (len > MAX_MESSAGE_LENGTH) {
        output_buffer[MAX_MESSAGE_LENGTH] = '\0';
        log_error("Message truncated from %s:%d", ip, listen_port);
    }
    log_info("Message from %s:%d: %s", ip, listen_port, output_buffer);
    return RECV_OK;
}