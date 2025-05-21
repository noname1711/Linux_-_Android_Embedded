#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/posix_queue"
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

int main() {
    mqd_t mq;
    char buffer[MSG_BUFFER_SIZE];
    
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_curmsgs = 0
    };

    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("POSIX Writer: Send messages (type 'exit' to quit)\n");
    
    while (1) {
        printf("Enter message: ");
        fflush(stdout);
        
        if (fgets(buffer, MSG_BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (mq_send(mq, buffer, strlen(buffer)+1, 0) == -1) {
            perror("mq_send");
            continue;
        }
        
        if (strcmp(buffer, "exit") == 0) {
            printf("Sent exit command. Exiting...\n");
            break;
        }
    }

    mq_close(mq);
    return 0;
}