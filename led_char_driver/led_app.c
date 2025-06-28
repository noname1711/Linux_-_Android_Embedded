#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = open("/dev/led_cdev", O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char cmd[8];
    printf("Enter command (on/off): ");
    scanf("%s", cmd);

    write(fd, cmd, strlen(cmd));

    close(fd);
    return 0;
}
