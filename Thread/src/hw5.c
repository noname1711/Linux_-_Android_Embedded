#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int shared_data = 0;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *reader(void *arg){
    pthread_rwlock_rdlock(&rwlock);
    printf("Reader %ld: read value %d\n", (long)arg, shared_data);
    sleep(1); 
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

void *writer(void *arg){
    pthread_rwlock_wrlock(&rwlock);
    shared_data++;
    printf("Writer %ld: incremented value to %d\n", (long)arg, shared_data);
    sleep(1);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

int main(){
    pthread_t r[5], w[2];
    for (long i = 0; i < 5; i++) {
        pthread_create(&r[i], NULL, reader, (void*)(i + 1));
    }
    for (long i = 0; i < 2; i++) {
        pthread_create(&w[i], NULL, writer, (void*)(i + 1));
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(r[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(w[i], NULL);
    }
    pthread_rwlock_destroy(&rwlock);
    printf("Final value of shared_data: %d\n", shared_data);
    return 0;
}