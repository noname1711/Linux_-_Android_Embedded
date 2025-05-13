#ifndef CONNECTION_H
#define CONNECTION_H

#include <netinet/in.h>

#define MAX_CONNECTIONS 10

typedef struct {
    int socket_fd;
    struct sockaddr_in addr;
    int listening_port;
    int id;
} Connection;

extern Connection connections[MAX_CONNECTIONS];
extern int conn_count;

void add_connection(int socket_fd, struct sockaddr_in addr, int listening_port);
int get_peer_listen_port(int socket_fd);
void remove_connection(int id);
void list_connections();
int get_socket_by_id(int id);
int get_id_by_socket(int socket_fd);
int get_connection_index_by_id(int id);
void cleanup_connections();

#endif
