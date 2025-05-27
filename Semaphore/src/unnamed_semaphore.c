#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define DATA_SIZE 1024

// Shared data and semaphore
char shared_data[DATA_SIZE];
sem_t sem;

// Producer thread function
void* producer(void* arg) {
    printf("Producer: Preparing data...\n");
    snprintf(shared_data, DATA_SIZE, "Data produced by thread %ld", (long)arg);
    sleep(1); // Simulate work
    printf("Producer: Data ready!\n");

    sem_post(&sem); // Signal that data is ready
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    (void)arg; // Explicitly mark unused parameter
    sem_wait(&sem); // Wait for data to be ready
    printf("Consumer: Received data: \"%s\"\n", shared_data);
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize unnamed semaphore (shared between threads, initial value = 0)
    sem_init(&sem, 0, 0);

    // Create threads
    pthread_create(&producer_thread, NULL, producer, (void*)1);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Cleanup
    sem_destroy(&sem);
    return 0;
}