#ifndef CONNECTION_H
#define CONNECTION_H

#include <netinet/in.h>

typedef struct {
    int id;
    int socket_fd;
    struct sockaddr_in addr;
} PeerInfo;

void add_connection(int socket_fd, struct sockaddr_in addr);
void remove_connection(int id);
void list_connections();
int get_socket_by_id(int id);
int get_id_by_socket(int socket_fd);
void cleanup_connections();

#endif
