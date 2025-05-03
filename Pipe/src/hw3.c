#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main( int argc, char *argv[]){
    int fd[2];
    char buffer[512];
    pid_t pid;
    if (argc != 2){
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *msg = argv[1];
    if (strlen(msg) > sizeof(buffer) - 1){
        fprintf(stderr, "Message is too long\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0){ 
        close(fd[1]); 
        sleep(1);
        ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
        if (bytesRead == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0'; 
        printf("Child process received: \"%s\" with length: %zu\n", buffer, strlen(buffer));
        close(fd[0]);
    } else {
        close(fd[0]); 
        ssize_t bytesWritten = write(fd[1], msg, strlen(msg));
        printf("Parent process sent to child: %s\n",msg);
        if (bytesWritten == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fd[1]); 
        wait(NULL); 
    }
    return 0;
}