#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "cli.h"
#include "connection.h"
#include "mes.h"
#include "myLog.h"
#include "mySocket.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int listener = setup_listener_socket(port);
    if (listener < 0) return 1;

    show_menu();
    log_info("Listening on port %d", port);

    fd_set master, read_fds;
    int fdmax = listener;
    FD_ZERO(&master);
    FD_SET(STDIN_FILENO, &master);
    FD_SET(listener, &master);

    char command[1024];

    while (1) {
        printf("---> ");
        fflush(stdout); 

        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        for (int i = 0; i <= fdmax; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == STDIN_FILENO) {
                    if (fgets(command, sizeof(command), stdin) == NULL) {
                        printf("\n");
                        continue;
                    }
                    command[strcspn(command, "\n")] = 0;

                    if (strncmp(command, "help", 4) == 0) {
                        show_help();
                    } else if (strncmp(command, "myip", 4) == 0) {
                        printf("Your IP: %s\n", get_my_ip());
                    } else if (strncmp(command, "myport", 6) == 0) {
                        printf("Your port: %d\n", port);
                    } else if (strncmp(command, "connect", 7) == 0) {
                        char ip[100]; int cport;
                        if (sscanf(command, "connect %s %d", ip, &cport) == 2) {
                            int peer_sock = create_connection(ip, cport);
                            if (peer_sock > 0) {
                                struct sockaddr_in addr;
                                addr.sin_family = AF_INET;
                                addr.sin_port = htons(cport);
                                inet_pton(AF_INET, ip, &addr.sin_addr);
                                add_connection(peer_sock, addr);
                                FD_SET(peer_sock, &master);
                                if (peer_sock > fdmax) fdmax = peer_sock;
                                log_info("Connected to %s:%d", ip, cport);
                            } else {
                                log_error("Failed to connect");
                            }
                        } else {
                            printf("Usage: connect <IP> <port>\n");
                        }
                    } else if (strncmp(command, "list", 4) == 0) {
                        list_connections();
                    } else if (strncmp(command, "terminate", 9) == 0) {
                        int id;
                        if (sscanf(command, "terminate %d", &id) == 1) {
                            int s = get_socket_by_id(id);
                            if (s > 0) {
                                close(s);
                                FD_CLR(s, &master);
                                remove_connection(id);
                                log_info("Connection %d terminated", id);
                            } else {
                                log_error("Invalid connection ID");
                            }
                        } else {
                            printf("Usage: terminate <id>\n");
                        }
                    } else if (strncmp(command, "send", 4) == 0) {
                        int id;
                        char msg[1000];
                        if (sscanf(command, "send %d %[^\n]", &id, msg) == 2) {
                            send_message(id, msg);
                        } else {
                            printf("Usage: send <id> <message>\n");
                        }
                    } else if (strncmp(command, "exit", 4) == 0) {
                        cleanup_connections();
                        log_info("Exiting...");
                        exit(0);
                    } else {
                        printf("Unknown command. Type 'help' for a list of commands.\n");
                    }
                } else if (i == listener) {
                    struct sockaddr_in remote;
                    socklen_t len = sizeof(remote);
                    int newfd = accept(listener, (struct sockaddr*)&remote, &len);
                    if (newfd >= 0) {
                        add_connection(newfd, remote);
                        FD_SET(newfd, &master);
                        if (newfd > fdmax) fdmax = newfd;
                        log_info("New peer connected from %s:%d",
                                 inet_ntoa(remote.sin_addr), ntohs(remote.sin_port));
                    } else {
                        perror("accept");
                    }
                } else {
                    char buffer[1024];
                    int len = recv(i, buffer, sizeof(buffer) - 1, 0);
                
                    if (len <= 0) {
                        if (len == 0) {
                            log_info("Peer disconnected (socket %d)", i);
                        } else {
                            log_error("recv() error on socket %d", i);
                        }
                
                        int id = get_id_by_socket(i);
                        if (id >= 0) remove_connection(id);
                        close(i);
                        FD_CLR(i, &master);
                    } else {
                        buffer[len] = '\0';
                        log_info("Message from ID %d: %s", get_id_by_socket(i), buffer);
                    }
                }
            }
        }
    }

    return 0;
}
