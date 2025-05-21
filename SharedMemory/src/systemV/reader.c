#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 256
#define IPC_KEY_FILE "/tmp/systemV_ipc_key"

struct message {
    long mtype;
    char mtext[MSG_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct message msg;
    
    if ((key = ftok(IPC_KEY_FILE, 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    
    if ((msgid = msgget(key, 0644)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("SystemV Reader: Waiting for messages...\n");
    
    while (1) {
        if (msgrcv(msgid, &msg, MSG_SIZE, 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        
        printf("Received: %s\n", msg.mtext);
        if (strcmp(msg.mtext, "exit") == 0) break;
    }
    
    return 0;
}