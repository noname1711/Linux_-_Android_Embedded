#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *filename = "./text/checkvar.txt";
    const char *data = "Goodluck hungle.\n";
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        printf("Error");
        return 1;
    }
    write(fd, data, strlen(data));
    close(fd);

    struct stat info;

    if (stat(filename, &info) != 0) {
        perror("Can't get info");
        return 1;
    }
    printf("Name: %s\n", filename);
    printf("Size: %ld byte\n", info.st_size);
    printf("Last edited: %s", ctime(&info.st_mtime));
    printf("File's style: ");
    if (S_ISREG(info.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(info.st_mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(info.st_mode)) {
        printf("Symbolic link\n");
    } else if (S_ISCHR(info.st_mode)) {
        printf("Character device\n");
    } else if (S_ISBLK(info.st_mode)) {
        printf("Block device\n");
    } else if (S_ISFIFO(info.st_mode)) {
        printf("FIFO\n");
    } else if (S_ISSOCK(info.st_mode)) {
        printf("Socket\n");
    } else {
        printf("Other file\n");
    }
    return 0;
}
