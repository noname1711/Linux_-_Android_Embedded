#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
    int fd[2];
    pid_t pid;
    char buffer[512];
    char *message = "Hello, this is a message from the parent process.\n";
    if(pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(pid == 0){
        close(fd[1]); 
        sleep(1);
        ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
        if (bytesRead >= 0) {
            buffer[bytesRead] = '\0'; 
            printf("Child process received: %s", buffer);
        } else {
            perror("read");
        }
        close(fd[0]);
    } else {
        close(fd[0]); 
        printf("Parent process sending to child process: %s", message);
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        wait(NULL); 
    }
    return 0;
}