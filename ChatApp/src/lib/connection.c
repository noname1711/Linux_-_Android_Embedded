#include "connection.h"
#include "myLog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


Connection connections[MAX_CONNECTIONS];
int conn_count = 0;    

void add_connection(int socket_fd, struct sockaddr_in addr, int listening_port) {
    if (conn_count >= MAX_CONNECTIONS) {
        log_error("Connection limit reached");
        return;
    }
    int new_id = 1;
    while (1) {
        int id_exists = 0;
        for (int i = 0; i < conn_count; i++) {
            if (connections[i].id == new_id) {
                id_exists = 1;
                break;
            }
        }
        if (!id_exists) break;
        new_id++;
    }
    connections[conn_count].socket_fd = socket_fd;
    connections[conn_count].addr = addr;
    connections[conn_count].listening_port = listening_port;
    connections[conn_count].id = new_id;
    conn_count++;
    log_info("Added new connection ID %d", new_id);
}


int get_peer_listen_port(int socket_fd) {
    for (int i = 0; i < conn_count; i++) {
        if (connections[i].socket_fd == socket_fd) {
            return connections[i].listening_port;
        }
    }
    return -1; 
}

void remove_connection(int id) {
    for (int i = 0; i < conn_count; i++) {
        if (connections[i].id == id) {
            for (int j = i; j < conn_count - 1; j++) {
                connections[j] = connections[j + 1];
            }
            conn_count--;
            log_info("Removed connection ID %d", id);
            return;
        }
    }
    log_error("Connection ID %d not found", id);
}

void list_connections() {
    printf("ID\tIP Address\tPort\tListening Port\n");
    for (int i = 0; i < conn_count; ++i) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(connections[i].addr.sin_addr), ip, sizeof(ip));
        printf("%d\t%s\t%d\t%d\n",
               connections[i].id,
               ip,
               ntohs(connections[i].addr.sin_port),
               connections[i].listening_port);
    }
}

int get_socket_by_id(int id) {
    for (int i = 0; i < conn_count; ++i) {
        if (connections[i].id == id) {
            return connections[i].socket_fd;
        }
    }
    return -1; 
}

int get_id_by_socket(int socket_fd) {
    for (int i = 0; i < conn_count; ++i) {
        if (connections[i].socket_fd == socket_fd) {
            return connections[i].id;
        }
    }
    return -1; 
}

void cleanup_connections() {
    for (int i = 0; i < conn_count; ++i) {
        close(connections[i].socket_fd); 
    }
    conn_count = 0; 
}

int get_connection_index_by_id(int id) {
    for (int i = 0; i < conn_count; i++) {
        if (connections[i].id == id) {
            return i;
        }
    }
    return -1;
}