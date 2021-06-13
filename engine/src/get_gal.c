#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "fh.h"

extern struct galaxy_data galaxy;

void
get_galaxy_data(void) {
    const char *filename = "galaxy.dat";
    struct stat sb;
    int         fd;
    ssize_t     bytesRead;

    if (stat(filename, &sb) != 0) {
        abend("\n\tCannot stat '%s'\n\n", filename);
    } else if (sb.st_size != sizeof(struct galaxy_data)) {
        abend("\n\tFile '%s' has size %d\n\tExpect sizeof(galaxy_data) %d\n", filename, sizeof(struct galaxy_data));
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        abend("\n\tCannot open '%s' for reading!\n\n", filename);
    }
    bytesRead = read(fd, &galaxy, sizeof(struct galaxy_data));
    if (bytesRead != sizeof(struct galaxy_data)) {
        abend("\n\tCannot read data in file '%s'!\n\n", filename);
    }
    close(fd);

    return;
}
