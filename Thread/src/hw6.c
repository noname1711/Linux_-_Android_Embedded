#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

long long size = 1000000;
long long a[1000000];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
long long sum = 0;

void* compute_sum(void* arg) {
    int thread_id = *(int*)arg;
    free(arg); 
    int start = thread_id * (size / 4);
    int end = (thread_id + 1) * (size / 4);
    printf("Thread %d compute from %d to %d...\n", thread_id, start, end - 1);
    long long local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += a[i];
    }
    printf("Thread %d done: local_sum = %lld\n", thread_id, local_sum);
    pthread_mutex_lock(&lock);
    sum += local_sum;
    printf("Thread %d add to global sum. Current total: %lld\n", thread_id, sum);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    for (int i = 0; i < size; i++) {
        a[i] = i;
    }
    pthread_t tid[4];
    for (int i = 0; i < 4; i++) {
        int* id = malloc(sizeof(int));  
        *id = i;
        pthread_create(&tid[i], NULL, compute_sum, id);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    printf("Final total: %lld\n", sum);
    return 0;
}