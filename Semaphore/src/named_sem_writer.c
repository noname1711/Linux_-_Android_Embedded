#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define SEM_NAME "/my_named_semaphore"
#define FILE_NAME "shared.txt"

int main() {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        perror("fopen failed");
        sem_close(sem);
        return 1;
    }

    const char *data = "Hello from Program A (Writer)!";
    fputs(data, file);
    fclose(file);

    printf("Writer: Data written to file. Signalling semaphore...\n");
    sem_post(sem); // Signal that data is ready

    sem_close(sem);
    return 0;
}