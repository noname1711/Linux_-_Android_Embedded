#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_CS "fifo_cs"
#define FIFO_SC "fifo_sc"
#define BUFFER_SIZE 512

int main() {
    const char* message = getenv("MES");
    if (!message) {
        fprintf(stderr, "Error: MES environment variable not set.\n");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];

    int fd_write = open(FIFO_CS, O_WRONLY);
    int fd_read = open(FIFO_SC, O_RDONLY);

    if (fd_write == -1 || fd_read == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    write(fd_write, message, strlen(message));
    printf("Client sent: '%s'\n", message);

    ssize_t bytesRead = read(fd_read, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Client received: '%s'\n", buffer);
    }

    close(fd_write);
    close(fd_read);

    return 0;
}
