#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>

#define QUEUE_NAME  "/my_message_queue"
#define MAX_SIZE    1024

int main() {
    mqd_t mq;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; 
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_send(mq, "Viet", strlen("Viet") + 1, 10);
    mq_send(mq, "Hung", strlen("Hung") + 1, 5);
    mq_send(mq, "Le", strlen("Le") + 1, 20);

    mq_close(mq);
    return 0;
}