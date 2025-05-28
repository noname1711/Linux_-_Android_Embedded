#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>

#define QUEUE_NAME  "/my_message_queue"
#define MAX_SIZE    1024

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];
    unsigned int priority;
    
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority);
        if (bytes_read >= 0) {
            printf("Received: %s with priority %u\n", buffer, priority);
        } else {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}