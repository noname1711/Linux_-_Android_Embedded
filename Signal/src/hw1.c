#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int count = 0;

void handler (int sig){
    count++;
    printf(" SIGINT (%d) received\n", sig);
    if(count >=3){
        printf("Exit program after 3 times");
        exit(0);
    }
}

int main(){
    signal(SIGINT, handler);
    printf("Press Ctrl+C to send SIGINT signal\n");
    while(1){
        pause();
    }
    return 0;
}