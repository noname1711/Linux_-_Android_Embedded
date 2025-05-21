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
    
    // Tạo key từ file cố định
    if ((key = ftok(IPC_KEY_FILE, 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    
    // Tạo message queue
    if ((msgid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("SystemV Writer: Send messages (type 'exit' to quit)\n");
    msg.mtype = 1;
    
    while (1) {
        printf("Enter message: ");
        fgets(msg.mtext, MSG_SIZE, stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
        
        if (strcmp(msg.mtext, "exit") == 0) break;
        
        if (msgsnd(msgid, &msg, strlen(msg.mtext)+1, 0) == -1) {
            perror("msgsnd");
        }
    }
    
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
    }
    
    return 0;
}