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
    fprintf(fp, "\t\"turn_number\": %d,\n", galaxy.turn_number);
    fprintf(fp, "\t\"d_num_species\": %d,\n", galaxy.d_num_species);
    fprintf(fp, "\t\"num_species\": %d,\n", galaxy.num_species);
    fprintf(fp, "\t\"radius\": %d,\n", galaxy.radius);
    fprintf(fp, "\t\"min_radius\": %d,\n", MIN_RADIUS);
    fprintf(fp, "\t\"max_radius\": %d,\n", MAX_RADIUS);
    fprintf(fp, "\t\"std_num_stars\": %d,\n", STANDARD_NUMBER_OF_STAR_SYSTEMS);
    fprintf(fp, "\t\"min_stars\": %d,\n", MIN_STARS);
    fprintf(fp, "\t\"max_stars\": %d,\n", MAX_STARS);
    fprintf(fp, "\t\"std_num_species\": %d,\n", STANDARD_NUMBER_OF_SPECIES);
    fprintf(fp, "\t\"min_species\": %d,\n", MIN_SPECIES);
    fprintf(fp, "\t\"max_species\": %d,\n", MAX_SPECIES);
    fprintf(fp, "\t\"max_items\": %d,\n", MAX_ITEMS);
    fprintf(fp, "\t\"max_locations\": %d,\n", MAX_LOCATIONS);
    fprintf(fp, "\t\"max_transactions\": %d,\n", MAX_TRANSACTIONS);
    fprintf(fp, "\t\"num_commands\": %d,\n", NUM_COMMANDS);
    fprintf(fp, "\t\"num_contact_words\": %d,\n", NUM_CONTACT_WORDS);
    fprintf(fp, "\t\"num_ship_classes\": %d\n", NUM_SHIP_CLASSES);
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
