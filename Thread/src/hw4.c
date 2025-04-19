#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int count_chan = 0;
int count_le = 0;
int a[100];

void *chan(void *arg){
    int* arr = (int*)arg;
    for (int i = 0; i<100; i++){
        if ( arr[i] % 2 == 0){
            count_chan++;
        }
    }
    return NULL;
}

void *le(void *arg){
    int* arr = (int*)arg;
    for (int i = 0; i<100; i++){
        if ( arr[i] % 2 != 0){
            count_le++;
        }
    }
    return NULL;
}

int main(){
    pthread_t thread1, thread2;
    for (int i = 0; i<100; i++){
        a[i] = rand() % 100;
    }
    for (int i = 0; i<100; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    pthread_create(&thread1, NULL, chan, (void*)a);
    pthread_create(&thread2, NULL, le, (void*)a);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Count of even numbers: %d\n", count_chan);
    printf("Count of odd numbers: %d\n", count_le);
    return 0;
}