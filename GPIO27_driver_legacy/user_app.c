#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/led_cdev"

int main(int argc, char *argv[]) {
    int fd;
    if (argc != 2) {
        printf("Usage: %s <on|off>\n", argv[0]);
        return 1;
    }

    fd = open(DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
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
