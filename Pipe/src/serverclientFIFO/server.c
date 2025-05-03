#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_CS "fifo_cs" // client to server
#define FIFO_SC "fifo_sc" // server to client
#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
    const char* message = getenv("MES");
    if (!message) {
        fprintf(stderr, "Error: MES environment variable not set.\n");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    mkfifo(FIFO_CS, 0666);
    mkfifo(FIFO_SC, 0666);

    printf("Server: Waiting for message from client...\n");

    int fd_read = open(FIFO_CS, O_RDONLY);
    int fd_write = open(FIFO_SC, O_WRONLY);

    if (fd_read == -1 || fd_write == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead = read(fd_read, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Server received: '%s'\n", buffer);

        write(fd_write, message, strlen(message));
        printf("Server sent: '%s'\n", message);
    }

    close(fd_read);
    close(fd_write);

    unlink(FIFO_CS);
    unlink(FIFO_SC);

    return 0;
}
