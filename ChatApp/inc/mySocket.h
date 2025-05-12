#ifndef MYSOCKET_H
#define MYSOCKET_H

int setup_listener_socket(int port);
char* get_my_ip();
int create_connection(const char* ip, int port);

#endif
