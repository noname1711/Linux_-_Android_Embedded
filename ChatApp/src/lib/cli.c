#include <stdio.h>
#include "cli.h"

void show_help() {
    printf("================================================\n");
    printf("The list of commands:\n");
    printf("help                - Show this help message\n");
    printf("myip                - Show local IP address\n");
    printf("myport              - Show listening port\n");
    printf("connect <IP> <port> - Connect to another peer\n");
    printf("list                - List all active connections\n");
    printf("terminate <id>      - Terminate connection by ID\n");
    printf("send <id> <message> - Send message to peer\n");
    printf("exit                - Exit the application\n");
    printf("================================================\n");
}

void show_menu() {
    printf("================ Chat Application =================\n");
    printf("Type a command (type 'help' for a list of commands):\n");
}
