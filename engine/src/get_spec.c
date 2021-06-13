#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "fh.h"

/* Routine "get_species_data" will read in data files for all species,
 *      "save_species_data" will write all data that has been modified, and
 *      "free_species_data" will free memory used for all species data. */

/* Additional memory must be allocated for routines that build ships or
 * name planets. Here are the default 'extras', which may be changed, if
 * necessary, by the main program. */

long extra_namplas = NUM_EXTRA_NAMPLAS;
long extra_ships   = NUM_EXTRA_SHIPS;

void
get_species_data(void) {
    extern struct galaxy_data galaxy;
    extern int data_in_memory[MAX_SPECIES];
    extern int data_modified[MAX_SPECIES];
    extern struct nampla_data *namp_data[MAX_SPECIES];
    extern int num_new_namplas[MAX_SPECIES];
    extern int num_new_ships[MAX_SPECIES];
    extern struct species_data spec_data[MAX_SPECIES];

    memset(spec_data, 0, sizeof(spec_data));

    for (int species_index = 0; species_index < MAX_SPECIES; species_index++) {
        data_in_memory[species_index] = FALSE;
        data_modified[species_index] = FALSE;
        num_new_namplas[species_index] = 0;
        num_new_ships[species_index]   = 0;
    }

    for (int species_index = 0; species_index < galaxy.num_species; species_index++) {
        char filename[16];
        struct stat sb;
        int fd;
        ssize_t     bytesRead;
        struct species_data *sp = &spec_data[species_index];

        sprintf(filename, "sp%02d.dat", species_index + 1);

        if (stat(filename, &sb) != 0 || sb.st_size == 0) {
            sp->pn = 0; /* Extinct! */
            continue;
        }

        fd = open(filename, O_RDONLY);
        if (fd < 0) {
            abend("\n\tCannot open '%s' for reading!\n\n", filename);
        }

        bytesRead = read(fd, sp, sizeof(struct species_data));
        if (bytesRead != sizeof(struct species_data)) {
            abend("\n\tCannot read species record in file '%s'!\n\n", filename);
        }

        namp_data[species_index] = (struct nampla_data *)calloc(sp->num_namplas + extra_namplas, sizeof(struct nampla_data));
        if (namp_data[species_index] == 0) {
            abend("\nCannot allocate enough memory for nampla data from file '%s'!\n\n", filename);
        }
        bytesRead = read(fd, namp_data[species_index], sp->num_namplas * sizeof(struct nampla_data));
        if (bytesRead != sp->num_namplas * sizeof(struct nampla_data)) {
            abend("\n\tCannot read nampla data from file '%s'!\n\n", filename);
        }

        ship_data[species_index] = (struct ship_data *)calloc(sp->num_ships + extra_ships, sizeof(struct ship_data));
        if (ship_data[species_index] == 0) {
            abend("\nCannot allocate enough memory for ship data from file '%s'!\n\n", filename);
        }
        if (sp->num_ships > 0) {
            bytesRead = read(fd, ship_data[species_index], sp->num_ships * sizeof(struct ship_data));
            if (bytesRead != sp->num_ships * sizeof(struct ship_data)) {
                fprintf(stderr, "\nCannot read ship data into memory!\n\n");
                exit(-1);
            }
        }

        close(fd);

        data_in_memory[species_index]  = TRUE;
    }
}
