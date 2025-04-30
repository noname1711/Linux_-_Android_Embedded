#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>

void handler(int signum){
    if (signum == SIGUSR1){
        printf("Received signal from parent\n");    
    }
}

int main(){
    pid_t pid = fork();
    if (pid < 0){
        printf("Fork failed");
        exit(1);
    }
    if (pid == 0){
        signal(SIGUSR1, handler);
        printf("Child process %d ready\n", getpid());
        while(1){
            pause(); 
        }
    }else {
        sleep(1);  //wait child setup signal 
        for(int i = 0; i < 5; i++){
            sleep(2);
            kill(pid, SIGUSR1);
        }  
        sleep(1);  //wait child done last signal     
        kill(pid, SIGKILL);
    }
    return 0;
}