#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <on|off>\n", argv[0]);
        return 1;
    }

    fd = open("/dev/ledctl", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open /dev/ledctl");
        return 1;
    }

    if (write(fd, argv[1], strlen(argv[1])) < 0) {
        perror("Failed to write to device");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
