#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <errno.h>

int sigint_received = 0;
int sigterm_received = 0;

void handler(int signo) {
    if (signo == SIGINT) {
        sigint_received = 1;
    } else if (signo == SIGTERM) {
        sigterm_received = 1;
    }
}

int main() {
    struct sigaction sa;
    char buf[1024];

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Enter data and Ctrl+C (SIGINT). Send SIGTERM to exit.\n");
    printf("PID: %d\n", getpid());

    while (1) {
        if (sigterm_received) {
            printf(" SIGTERM received. Exit program.\n");
            break;
        }

        if (sigint_received) {
            printf(" SIGINT received.\n");
            sigint_received = 0; 
        }
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 1; 
        timeout.tv_usec = 0;

        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(buf, sizeof(buf), stdin)) {
                printf("Enter: %s", buf);
            }
        } else if (ret == -1) {
            if (errno == EINTR) {
                continue;
            }
            printf("select failed");
            break;
        }
    }
    return 0;
}
