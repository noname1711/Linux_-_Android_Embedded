#include "connection.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define MAX_CONN 10

static PeerInfo connections[MAX_CONN];
static int conn_count = 0;
static int next_id = 1;

void add_connection(int socket_fd, struct sockaddr_in addr) {
    if (conn_count < MAX_CONN) {
        connections[conn_count].id = next_id++;
        connections[conn_count].socket_fd = socket_fd;
        connections[conn_count].addr = addr;
        conn_count++;
    }
}

void remove_connection(int id) {
    for (int i = 0; i < conn_count; ++i) {
        if (connections[i].id == id) {
            for (int j = i; j < conn_count - 1; ++j) {
                connections[j] = connections[j + 1];
            }
            conn_count--;
            return;
        }
    }
}

void list_connections() {
    printf("ID\tIP Address\tPort\n");
    for (int i = 0; i < conn_count; ++i) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(connections[i].addr.sin_addr), ip, sizeof(ip));
        printf("%d\t%s\t%d\n", connections[i].id, ip, ntohs(connections[i].addr.sin_port));
    }
}

int get_socket_by_id(int id) {
    for (int i = 0; i < conn_count; ++i) {
        if (connections[i].id == id)
            return connections[i].socket_fd;
    }
    return -1;
}

int get_id_by_socket(int socket_fd) {
    for (int i = 0; i < conn_count; ++i) {
        if (connections[i].socket_fd == socket_fd)
            return connections[i].id;
    }
    return -1;
}

void cleanup_connections() {
    for (int i = 0; i < conn_count; ++i) {
        close(connections[i].socket_fd);
    }
    conn_count = 0;
}
