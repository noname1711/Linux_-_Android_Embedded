#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>

#define SEM_NAME "/my_named_semaphore"
#define FILE_NAME "shared.txt"

int main() {
    sem_t *sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    printf("Reader: Waiting for data...\n");
    sem_wait(sem); // Wait until data is ready

    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        perror("fopen failed");
        sem_close(sem);
        return 1;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    printf("Reader: Received data: \"%s\"\n", buffer);
    fclose(file);

    sem_close(sem);
    sem_unlink(SEM_NAME); // Cleanup
    return 0;
}