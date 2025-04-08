#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 5){ 
        printf("Error");
        return 1;
    }
    char *filename = argv[1];
    int num_bytes = atoi(argv[2]);
    char mode = argv[3][0];
    char *data = argv[4];
    int fd;
    if (mode == 'w') {
        fd = open(filename, O_WRONLY | O_CREAT |O_APPEND, 0644);  
        if (fd == -1) return 1;
        write(fd, data, num_bytes);
        close(fd);
    } else if (mode == 'r') {
        fd = open(filename, O_RDONLY);
        if (fd == -1) return 1;
        char buf[1024];
        int bytes_read = read(fd, buf, num_bytes);
        write(STDOUT_FILENO, buf, bytes_read);
        close(fd);
    } else {
        return 1;
    }
    return 0;
}
