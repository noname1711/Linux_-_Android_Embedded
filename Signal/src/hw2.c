#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int count = 0;

void handler(int sig){
    count++;
    printf("Timer: %d seconds\n", count);
    if(count == 10){
        printf("Exiting after 10 seconds\n");
        exit(0);
    }
    alarm(1); 
}

int main(){
    signal(SIGALRM, handler);
    alarm(1);
    while(1){
        pause();
    }
    return 0;
}