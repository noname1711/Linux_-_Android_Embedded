#define _POSIX_C_SOURCE 200809L   //detect POSIX including sigaction

#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>

void handler(int sig){
    printf(" SIGTSTP ignored\n");
    fflush(stdout);
}

int main(){
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if(sigaction(SIGTSTP, &sa, NULL) == -1){
        printf("sigaction error\n");
        exit(1);
    }
    printf("Running... (Press Ctrl+Z to send SIGTSTP)\n");
    while(1){
        pause(); 
    }
    return 0;
}