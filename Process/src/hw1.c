#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int retVal;
    retVal = fork();
    if(retVal == 0){
        printf("Child process: My PID is %d\n", getpid());
    }else if(retVal > 0){
        printf("PID of parent process: %d\n", getpid());
        printf("PID of my child: %d\n", retVal);
    }
    else{
        printf("Fork failed\n");
    }
    return 0;
}