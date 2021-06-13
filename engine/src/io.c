#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "fh.h"

void *
readFile(const char *filename) {
    struct stat sb;
    char *buf;
    int fd;
    ssize_t bytesRead;
    
    if (stat(filename, &sb) != 0) {
        fprintf(stderr, "\n\tCannot stat '%s'\n\n", filename);
        return 0;
    }

    buf = malloc(sb.st_size+1);
    if (buf == 0) {
        abend("\n\tOut of memory loading '%s'\n\n", filename);
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        abend("\n\tCannot open '%s' for reading!\n\n", filename);
    }
    bytesRead = read(fd, buf, sb.st_size);
    if (bytesRead != sb.st_size) {
        abend("\n\tCannot read data in file '%s'!\n\n", filename);
    }
	close(fd);

    buf[sb.st_size] = 0;
    return buf;
}
