#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define QUEUE_NAME "/posix_queue"
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

int main() {
    mqd_t mq;
    char buffer[MSG_BUFFER_SIZE];
    ssize_t bytes_read;
    
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_curmsgs = 0
    };

    mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("POSIX Reader: Waiting for messages...\n");
    
    while (1) {
        bytes_read = mq_receive(mq, buffer, MSG_BUFFER_SIZE, NULL);
        
        if (bytes_read < 0) {
            perror("mq_receive");
            continue;
        }
        
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);
        
        if (strcmp(buffer, "exit") == 0) {
            printf("Received exit command. Exiting...\n");
            break;
        }
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    return 0;
}