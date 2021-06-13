#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "fh.h"

/* In case gamemaster creates new star systems with Edit program. */
#define NUM_EXTRA_STARS    20

int num_stars, star_data_modified;

struct star_data *star_base;

void
get_star_data() {
    extern int num_stars;
    extern int star_data_modified;
    extern struct star_data *star_base;

    const char *filename = "stars.dat";
    struct stat sb;
    int         fd;
    ssize_t     bytesRead;
    int32_t     numStars;

    if (stat(filename, &sb) != 0) {
        abend("\n\tCannot stat '%s'\n\n", filename);
    } else if (sb.st_size == 0) {
        abend("\n\tFile '%s' is empty\n\n", filename);
    } else if (sb.st_size < sizeof(numStars)) {
        abend("\n\tFile '%s' does not have number of stars marker\n\n", filename);
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        abend("\n\tCannot open '%s' for reading!\n\n", filename);
    }

    bytesRead = read(fd, &numStars, sizeof(numStars));
    if (bytesRead != sizeof(numStars)) {
        abend("\n\tCannot read number of stars from file '%s'!\n\n", filename);
    }
    if (sizeof(numStars) + numStars * sizeof(struct star_data) != sb.st_size) {
        fprintf(stderr, "File '%s' is %8ld bytes\n", filename, (long int)sb.st_size);
        fprintf(stderr, "    marker    is %8ld bytes\n", (long int)sizeof(numStars));
        fprintf(stderr, "    star_data is %8ld bytes\n", (long int)sizeof(struct star_data));
        fprintf(stderr, "    num_stars is %8d bytes\n", numStars);
        fprintf(stderr, "    file size sb %8ld bytes\n", (long int)(sizeof(numStars) + numStars * sizeof(struct star_data)));
        abend("\n\tFile '%s' doesn't contain %d stars\n\n", filename, numStars, (sb.st_size - sizeof(numStars)) % sizeof(struct star_data));
    }

    // Globals. Sigh.
    num_stars = numStars;

    // allocate enough memory for all stars.
    star_base = (struct star_data *)calloc(numStars + NUM_EXTRA_STARS, sizeof(struct star_data));
    if (star_base == 0) {
        abend("\nCannot allocate enough memory for %d stars!\n\n", numStars + NUM_EXTRA_STARS);
    }

    bytesRead = read(fd, star_base, numStars * sizeof(struct star_data));
    if (bytesRead != numStars * sizeof(struct star_data)) {
        abend("\n\tCannot read data in file '%s'!\n\n", filename);
    }

    close(fd);

    star_data_modified = FALSE;
}
