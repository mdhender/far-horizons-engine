#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "fh.h"

/* In case gamemaster creates new star systems with Edit program. */
#define NUM_EXTRA_PLANETS    100

int num_planets, planet_data_modified;

struct planet_data *planet_base;

void
get_planet_data(void) {
    extern struct planet_data *planet_base;
    extern int num_planets;
    extern int planet_data_modified;

    const char *filename = "planets.dat";
    struct stat sb;
    int         fd;
    ssize_t     bytesRead;
    int32_t     numPlanets;

    if (stat(filename, &sb) != 0) {
        abend("\n\tCannot stat '%s'\n\n", filename);
    } else if (sb.st_size == 0) {
        abend("\n\tFile '%s' is empty\n\n", filename);
    } else if (sb.st_size < sizeof(numPlanets)) {
        abend("\n\tFile '%s' does not have number of planets marker\n\n", filename);
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        abend("\n\tCannot open '%s' for reading!\n\n", filename);
    }

    bytesRead = read(fd, &numPlanets, sizeof(numPlanets));
    if (bytesRead != sizeof(numPlanets)) {
        abend("\n\tCannot read number of planets from file '%s'!\n\n", filename);
    }
    if (sizeof(numPlanets) + numPlanets * sizeof(struct planet_data) != sb.st_size) {
        abend("\n\tFile '%s' doesn't contain %d planets\n\n", filename, numPlanets, (sb.st_size - sizeof(numPlanets)) % sizeof(struct planet_data));
    }

    // Globals. Sigh.
    num_planets = numPlanets;

    // allocate enough memory for all planets.
    planet_base = (struct planet_data *)calloc(numPlanets + NUM_EXTRA_PLANETS, sizeof(struct planet_data));
    if (planet_base == 0) {
        abend("\nCannot allocate enough memory for %d stars!\n\n", numPlanets + NUM_EXTRA_PLANETS);
    }

    bytesRead = read(fd, planet_base, numPlanets * sizeof(struct planet_data));
    if (bytesRead != numPlanets * sizeof(struct planet_data)) {
        abend("\n\tCannot read data in file '%s'!\n\n", filename);
    }

    close(fd);

    planet_data_modified = FALSE;
}
