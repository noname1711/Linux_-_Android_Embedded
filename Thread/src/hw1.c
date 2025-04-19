#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void *task1 (void *arg){
    printf("Thread 1: Hello from thread 1 ID: %lu\n", pthread_self());
    return NULL;
}

void *task2 (void *arg){
    printf("Thread 2: Hello from thread 2 ID: %lu\n", pthread_self());
    return NULL;
}

int main(){
    pthread_t thread1, thread2;
    if (pthread_create(&thread1, NULL, task1, NULL) != 0) {
        printf("Failed to create thread 1");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, task2, NULL) != 0) {
        printf("Failed to create thread 2");
        exit(1);
    }
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Main thread: 2 thread end\n");
    return 0;
}