#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void abend(const char *fmt, ...) {
    va_list ap;

    // print the global error, if any
    if (errno != 0) {
        fprintf(stderr, "\n%s\n", strerror(errno));
    }

    // print the termination message
    if (fmt != 0 || fmt[0] != 0) {
        char msg[4 * 1024]; // buffer for error
        va_start(ap, fmt);
        vsnprintf(msg, sizeof(msg) - 1, fmt, ap);
        va_end(ap);
        fprintf(stderr, "\n%s\n", msg);
    }

    exit(2);
}