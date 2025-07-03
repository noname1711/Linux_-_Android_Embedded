#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd;
    if (argc != 2) {
        printf("Usage: %s <on|off>\n", argv[0]);
        return 1;
    }

    fd = open("/dev/myled", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open /dev/myled");
        return 1;
    }

    ssize_t bytes = write(fd, argv[1], strlen(argv[1]));
    if (bytes < 0) {
        perror("write");
    }

    close(fd);
    return 0;
}
