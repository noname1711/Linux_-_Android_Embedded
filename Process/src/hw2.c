#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid;
    pid = fork();
    if (pid < 0) {
        printf("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        char *choice = getenv("CHOICE");
        if (choice != NULL) {
            if (choice[0] == '1') {
                char *args[] = {"ls", "-l", NULL};
                execvp("ls", args);
                printf("execvp ls failed\n");
                exit(1);
            } else if (choice[0] == '2') {
                char *args[] = {"date", NULL};
                execvp("date", args);
                printf("execvp date failed\n");
                exit(1);
            }
        }
    }
    else {
        printf("Wait 10s\n");
        sleep(10);
        printf("Child process ended.\n");
    }
    return 0;
}
