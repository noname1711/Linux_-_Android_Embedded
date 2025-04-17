#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
    char *choice = getenv("CHOICE");
    if (choice[0] != 'Z' && choice[0] != 'O') {
        printf("Invalid choice\n");
        return 1;
    }
    if (choice[0] == 'Z') {
        printf("Creating a zombie process...\n");
        pid_t pid = fork(); 
        if (pid < 0) {
            printf("fork failed");
            return 1;
        }
        else if (pid == 0) {
            printf("Child process: PID %d\n", getpid());
            sleep(3); 
            printf("Child process exiting...\n");
            exit(0);
        }
        else {
            while (1); 
            int status;
            pid_t wait_child = wait(&status);
        }
    } else if (choice[0] == 'O') {
        printf("Creating an orphan process...\n");
        pid_t pid = fork(); 
        if (pid < 0) {
            printf("fork failed");
            return 1;
        }
        else if (pid == 0) {
            printf("Child process sleeping and exiting after 10s\n");
            sleep(10); 
            exit(0);
        }
        else {
            printf("Parent process sleep less than child and exit\n");
            sleep(3);
            exit(0);
        }
    }
    return 0;
}