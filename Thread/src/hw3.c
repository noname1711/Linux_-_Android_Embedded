#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int data = 0;
int ready = 0;  //ready = 1 when data is ready

void *producer(void *arg) {
    for (int i = 0; i < 10; i++) {
        int random = rand() % 10 + 1;
        pthread_mutex_lock(&lock);
        data = random;
        printf("Producer produced: %d\n", data);
        ready = 1;  //data is ready
        pthread_cond_signal(&cond);  
        pthread_mutex_unlock(&lock);
        sleep(1);  
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 10; i++) {
        
        pthread_mutex_lock(&lock);
        while (ready == 0) {  
            pthread_cond_wait(&cond, &lock);  
        }
        printf("Consumer consumed: %d\n", data);
        ready = 0;  //data is consumed
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}

    
