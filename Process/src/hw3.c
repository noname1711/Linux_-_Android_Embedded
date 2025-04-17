#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/wait.h>

void sigusr1_handler(int signum) {
    printf("Child process get SIGUSR1 signal(id: %d)\n", signum);
}

int main() {
    pid_t pid = fork();  
    if (pid < 0) {
        printf("fork failed");
        return 1;
    }
    else if (pid == 0) {
        signal(SIGUSR1, sigusr1_handler);  
        printf("Child process wait signal\n");
        pause();  
        printf("Child process end\n");
    }
    else {
        sleep(2);  
        printf("Parent process send SIGUSR1 signal to child process (PID: %d)\n", pid);
        kill(pid, SIGUSR1);  
        wait(NULL); 
        printf("Parent process end\n");
    }
    return 0;
}