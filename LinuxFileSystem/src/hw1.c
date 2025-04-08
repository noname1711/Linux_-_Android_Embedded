#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>

int main(){
    int fd = open("./text/test.txt", O_RDWR | O_APPEND );
    if (fd == -1) {
        printf("Error opening file");
        return 1;
    }
    lseek(fd, 0, SEEK_SET);
    char *buf = " Le"; 
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}