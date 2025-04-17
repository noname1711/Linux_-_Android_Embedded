#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid < 0){
        printf("fork failed");
        return 1;
    }
    else if(pid == 0){
        printf("Child process (PID: %d) is running\n", getpid());
        sleep(2);
        printf("Child process end\n");
        exit(0); 
    } else{
        int status;
        pid_t wait_child = wait(&status);
        if(wait_child == -1){
            printf("wait failed\n");
            return 1;
        }
        if(WIFEXITED(status)){
            int exit_status = WEXITSTATUS(status);
            printf("Child process ended by exit\n");
            printf("Child process exit status: %d\n", exit_status);
        }else{
            printf("Child process didn't end by exit\n");
        }
    }       
    return 0;
}