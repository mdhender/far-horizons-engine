/*
 * Convert binary data files to portable JSON.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "fh.h"

extern int num_stars, num_planets;

int species_index, species_number;


struct galaxy_data   galaxy;
struct star_data *   star;
struct planet_data * planet;
struct species_data *species;
struct nampla_data * nampla_base, *nampla;
struct ship_data *   ship_base, *ship;

extern struct star_data *  star_base;
extern struct planet_data *planet_base;

void get_galaxy_data(void);
void get_planet_data(void);
void get_species_data(void);
void get_star_data(void);

const char *semver = "7.5.2";

void fgalaxy() {
    // create the output file
    FILE *fp = fopen("galaxy.json", "w");
    if (fp == NULL) {
        fprintf(stderr, "Cannot create output file 'galaxy.json'!\n");
        return;
    }
    fprintf(fp, "{\n");
    fprintf(fp, "\t\"semver\": \"%s\",\n", semver);
    fprintf(fp, "\t\"d_num_species\": %d,\n", galaxy.d_num_species);
    fprintf(fp, "\t\"num_species\": %d,\n", galaxy.num_species);
    fprintf(fp, "\t\"radius\": %d,\n", galaxy.radius);
    fprintf(fp, "\t\"turn_number\": %d,\n", galaxy.turn_number);
    fprintf(fp, "}\n");
    fclose(fp);
}

int
main(int argc, char *argv[]) {
    /* Get all binary data. */
    get_galaxy_data();
    get_star_data();
    get_planet_data();
    get_species_data();

    fgalaxy();

    return 0;
}
