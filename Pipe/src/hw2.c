#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void changeMes (char *msg){
    char *pos = strstr(msg, "parent");
    if (pos != NULL) {
        char new[512];
        strcpy(new, pos + strlen("parent"));
        strcpy(pos, "child 1");
        strcat(msg, new);
    }
}

int main(){
    int fd1[2]; //parent to child1
    int fd2[2]; //child 1 to child 2
    pid_t pid1, pid2;
    char buffer[512];

    char *msg = "Hello, this is a message from the parent process.\n";
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0) { 
        // Child 1
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid2 == 0) { 
            // Child 2
            close(fd1[1]);
            close(fd1[0]); 
            close(fd2[1]); 
            sleep(3);
            ssize_t bytesRead = read(fd2[0], buffer, sizeof(buffer) - 1);
            if (bytesRead >= 0) {
                buffer[bytesRead] = '\0'; 
                printf("Child 2 process received: %s", buffer);
            } else {
                perror("read");
            }
            close(fd2[0]);
        } else {
            // Child 1
            close(fd1[1]); 
            close(fd2[0]); 
            sleep(1);
            ssize_t bytesRead = read(fd1[0], buffer, sizeof(buffer) - 1);
            if (bytesRead >= 0) {
                buffer[bytesRead] = '\0'; 
                printf("Child 1 process received: %s", buffer);
                sleep(1);
                changeMes(buffer);
                write(fd2[1], buffer, strlen(buffer) + 1);
                printf("Child 1 process sending to child 2 process: %s", buffer);
            } else {
                perror("read");
            }
            close(fd1[0]);
            close(fd2[1]);
            wait(NULL); 
        }
    } else {
        // Parent
        close(fd1[0]); 
        close(fd2[0]); 
        close(fd2[1]);
        printf("Parent process sending to child 1 process: %s", msg);
        write(fd1[1], msg, strlen(msg) + 1);
        close(fd1[1]);
        wait(NULL);
    }
    return 0;   
}