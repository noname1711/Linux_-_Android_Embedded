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
                    } 
                    
                    else if (strncmp(command, "myip", 4) == 0) {
                        printf("Your IP: %s\n", get_my_ip());
                    } 
                    
                    else if (strncmp(command, "myport", 6) == 0) {
                        printf("Your port: %d\n", port);
                    } 
                    
                    else if (strncmp(command, "connect", 7) == 0) {
                        char ip[INET_ADDRSTRLEN];
                        int cport;
                        if (sscanf(command, "connect %s %d", ip, &cport) == 2) {
                            if (strcmp(ip, get_my_ip()) == 0 && cport == port) {
                                log_error("You cannot connect to yourself (%s:%d)", ip, cport);
                                continue; 
                            }
                            int duplicate_found = 0;
                            for (int i = 0; i < conn_count; ++i) {
                                char existing_ip[INET_ADDRSTRLEN];
                                inet_ntop(AF_INET, &(connections[i].addr.sin_addr), existing_ip, sizeof(existing_ip));
                                if (strcmp(ip, existing_ip) == 0 && cport == connections[i].listening_port) {
                                    log_error("Connection to %s:%d already exists", ip, cport);
                                    duplicate_found = 1; 
                                    break; 
                                }
                            }
                            if (duplicate_found) {
                                continue; 
                            }
                            int peer_sock = create_connection(ip, cport);
                            if (peer_sock > 0) {
                                struct sockaddr_in addr;
                                addr.sin_family = AF_INET;
                                addr.sin_port = htons(cport);
                                inet_pton(AF_INET, ip, &addr.sin_addr);
                                char listen_port_str[10];
                                snprintf(listen_port_str, sizeof(listen_port_str), "%d", port);
                                send(peer_sock, listen_port_str, strlen(listen_port_str), 0);
                                add_connection(peer_sock, addr, cport);
                                FD_SET(peer_sock, &master);
                                if (peer_sock > fdmax) fdmax = peer_sock;
                                log_info("Connected to %s:%d", ip, cport);
                            } else {
                                log_error("Failed to connect to %s:%d", ip, cport);
                            }
                        } else {
                            log_error("Usage: connect <IP> <port>");
                        }
                    } 
                    
                    else if (strncmp(command, "list", 4) == 0) {
                        list_connections();
                    } 
                    
                    else if (strncmp(command, "terminate", 9) == 0) {
                        int id;
                        if (sscanf(command, "terminate %d", &id) == 1) {
                            int index = -1;
                            char ip[INET_ADDRSTRLEN];
                            int peer_port = 0;
                            for (int i = 0; i < conn_count; ++i) {
                                if (connections[i].id == id) {
                                    index = i;
                                    inet_ntop(AF_INET, &(connections[i].addr.sin_addr), ip, sizeof(ip));
                                    peer_port = connections[i].listening_port;
                                    break;
                                }
                            }
                            if (index != -1) {
                                close(connections[index].socket_fd);
                                FD_CLR(connections[index].socket_fd, &master);
                                for (int j = index; j < conn_count - 1; ++j) {
                                    connections[j] = connections[j + 1];
                                }
                                conn_count--;
                                log_info("Connection %d with %s:%d terminated", id, ip, peer_port);
                            } else {
                                log_error("Connection with ID %d not found", id);
                            }
                        } else {
                            log_error("Usage: terminate <id>");
                        }
                    }
                    
                    else if (strncmp(command, "send", 4) == 0) {
                        int id;
                        char msg[1000];
                        if (sscanf(command, "send %d %[^\n]", &id, msg) == 2) {
                            send_message(id, msg); 
                        } else {
                            printf("Usage: send <id> <message>\n");
                        }
                    } 
                    
                    else if (strncmp(command, "exit", 4) == 0) {
                        for (int i = 0; i <= fdmax; i++) {
                            if (FD_ISSET(i, &master) && i != STDIN_FILENO && i != listener) {
                                close(i);
                                FD_CLR(i, &master);
                                char exit_msg[] = "EXIT";
                                send(i, exit_msg, sizeof(exit_msg), 0);
                            }
                        }
                        if (listener != -1) {
                            close(listener);
                            FD_CLR(listener, &master);
                        }
                        cleanup_connections();
                        log_info("Exiting...");
                        exit(0);
                    } 
                } 
                
                else if (i == listener) { 
                    struct sockaddr_in remote;
                    socklen_t len = sizeof(remote);
                    int newfd = accept(listener, (struct sockaddr*)&remote, &len);
                    if (newfd >= 0) {
                        
                        char buffer[1024];
                        int bytes_received = recv(newfd, buffer, sizeof(buffer) - 1, 0);
                        if (bytes_received > 0) {
                            buffer[bytes_received] = '\0';
                            int peer_listen_port = atoi(buffer); 
                            add_connection(newfd, remote, peer_listen_port);
                            FD_SET(newfd, &master);
                            if (newfd > fdmax) fdmax = newfd;
                            log_info("New peer connected from %s:%d (listening on port %d)",
                                    inet_ntoa(remote.sin_addr), ntohs(remote.sin_port), peer_listen_port);
                        } else {
                            log_error("Failed to receive listening port from peer");
                            close(newfd);
                        }
                    } else {
                        perror("accept");
                    }
                } else { 
                    char message_buffer[1024];
                    RecvStatus status = receive_message(i, message_buffer, sizeof(message_buffer));

                    if (status == RECV_DISCONNECT || status == RECV_EXIT) {
                        int id = get_id_by_socket(i);
                        if (id != -1) {
                            char ip[INET_ADDRSTRLEN];
                            struct sockaddr_in addr = connections[get_connection_index_by_id(id)].addr;
                            inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
                            
                            log_info("Peer %s:%d disconnected", 
                                    ip, 
                                    connections[get_connection_index_by_id(id)].listening_port);
                            
                            remove_connection(id);
                        }
                        close(i);
                        FD_CLR(i, &master);
                        if (i == fdmax) {
                            while (fdmax > 0 && !FD_ISSET(fdmax, &master))
                                fdmax--;
                        }
                    } 
                }
            }
        }
    }

    return 0;
}