#include "myLog.h"
#include <stdio.h>
#include <stdarg.h>

void log_info(const char* fmt, ...) {
    printf("[INFO] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void log_error(const char* fmt, ...) {
    printf("[ERROR] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}
