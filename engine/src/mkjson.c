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

int
main(int argc, char *argv[]) {
    FILE *fp;

    /* Get all binary data. */
    get_galaxy_data();
    get_star_data();
    get_planet_data();
    get_species_data();

    fp = fopen("galaxy.json", "w");
    if (fp == NULL) {
        fprintf(stderr, "Cannot create output file 'galaxy.json'!\n");
        return 2;
    }

    fprintf(fp, "{\n");
    fprintf(fp, "\t\"semver\": \"%s\",\n", semver);

    fprintf(fp, "\t\"galaxy\": {\n");
    fprintf(fp, "\t\t\"d_num_species\": %d,\n", galaxy.d_num_species);
    fprintf(fp, "\t\t\"num_species\": %d,\n", galaxy.num_species);
    fprintf(fp, "\t\t\"radius\": %d,\n", galaxy.radius);
    fprintf(fp, "\t\t\"turn_number\": %d,\n", galaxy.turn_number);
    fprintf(fp, "\t},\n");

    fprintf(fp, "\t\"stars\": [\n");
    fprintf(fp, "\t],\n");
    fclose(fp);

    fprintf(fp, "\t\"planets\": [\n");
    fprintf(fp, "\t]\n");

    fprintf(fp, "}\n");

    fclose(fp);

    return 0;
}
