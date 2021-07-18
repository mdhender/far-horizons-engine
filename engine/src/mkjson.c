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
isset(int species_number, int32_t set[]) {
    return set[(species_number - 1) / 32] & (1 << ((species_number - 1) % 32));
}

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
    fprintf(fp, "\t\t\"%s\": %d,\n", "d_num_species", galaxy.d_num_species);
    fprintf(fp, "\t\t\"%s\": %d,\n", "num_species", galaxy.num_species);
    fprintf(fp, "\t\t\"%s\": %d,\n", "radius", galaxy.radius);
    fprintf(fp, "\t\t\"%s\": %d,\n", "turn_number", galaxy.turn_number);
    fprintf(fp, "\t},\n");

    fprintf(fp, "\t\"stars\": [\n");
    num_stars = 1;
    for (int i = 0; i < num_stars; i++) {
        struct star_data *star = star_base + i;
        fprintf(fp, "\t\t{\n");
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "id", i);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "color", star->color);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "home_system", star->home_system);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "message", star->message);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "num_planets", star->num_planets);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "planet_index", star->planet_index);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "size", star->size);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "type", star->type);
        fprintf(fp, "\t\t\t\"%s\": [", "visited_by");
        for (int j = 0; j < NUM_CONTACT_WORDS; j++) {
            if (star->visited_by[j] != 0) {
                char *vsep = "";
                for (int sp = 1; sp <= MAX_SPECIES; sp++) {
                    if (isset(sp, star->visited_by)) {
                        fprintf(fp, "%s%d", vsep, sp);
                        vsep = ", ";
                    }
                }
                break;
            }
        }
        fprintf(fp, "],\n");
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "worm_here", star->worm_here);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "worm_x", star->worm_x);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "worm_y", star->worm_y);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "worm_z", star->worm_z);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "x", star->x);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "y", star->y);
        fprintf(fp, "\t\t\t\"%s\": %d\n", "z", star->z);
        if (i + 1 < num_stars) {
            fprintf(fp, "\t\t},\n");
        } else {
            fprintf(fp, "\t\t}\n");
        }
    }
    fprintf(fp, "\t],\n");

    fprintf(fp, "\t\"planets\": [\n");
    for (int i = 0; i < num_planets; i++) {
        struct planet_data *planet = planet_base + i;
        fprintf(fp, "\t\t{\n");
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "id", i);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "diameter", planet->diameter);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "econ_efficiency", planet->econ_efficiency);
        fprintf(fp, "\t\t\t\"%s\": [%d,%d,%d,%d],\n", "gas", planet->gas[0], planet->gas[1], planet->gas[2], planet->gas[3]);
        fprintf(fp, "\t\t\t\"%s\": [%d,%d,%d,%d],\n", "gas_percent", planet->gas_percent[0], planet->gas_percent[1], planet->gas_percent[2], planet->gas_percent[3]);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "gravity", planet->gravity);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "md_increase", planet->md_increase);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "message", planet->message);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "mining_difficulty", planet->mining_difficulty);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "pressure_class", planet->pressure_class);
        fprintf(fp, "\t\t\t\"%s\": %d,\n", "special", planet->special);
        fprintf(fp, "\t\t\t\"%s\": %d\n", "temperature_class", planet->temperature_class);
        if (i + 1 < num_planets) {
            fprintf(fp, "\t\t},\n");
        } else {
            fprintf(fp, "\t\t}\n");
        }
    }
    fprintf(fp, "\t],\n");

    fprintf(fp, "\t\"species\": [\n");
    fprintf(fp, "\t],\n");

    fprintf(fp, "\t\"locations\": [\n");
    fprintf(fp, "\t],\n");

    fprintf(fp, "\t\"transactions\": [\n");
    fprintf(fp, "\t]\n");

    fprintf(fp, "}\n");

    fclose(fp);

    return 0;
}


