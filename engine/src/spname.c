//
// Created by Michael Henderson on 2/28/2022.
//
// to build on Linux, run the following command
//    cc -no-pie spname.c utils.c get_gal.c get_star.c get_plan.c abend.c -o ../bin/spname

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define THIS_IS_MAIN

#include "spname.h"

struct galaxy_data galaxy;
int species_number;

int main(int argc, char *argv[]) {
    get_galaxy_data();
    get_species_data();

    updateSpeciesName("TheGoo", "The Goo");

    save_species_data();

    return 0;
}


void updateSpeciesName(const char *from, const char *to) {
    if (strlen(from) > 30) {
        fprintf(stderr, "error: from must be less than 31 characters long\n");
        exit(2);
    } else if (strlen(to) > 30) {
        fprintf(stderr, "error: to must be less than 31 characters long\n");
        exit(2);
    }

    for (int spidx = 0; spidx < galaxy.num_species; spidx++) {
        if (data_in_memory[spidx] && strcmp((char *) (spec_data[spidx].name), from) == 0) {
            memset(spec_data[spidx].name, 0, 32);
            strcpy((char *) (spec_data[spidx].name), to);
            data_modified[spidx] = TRUE;
            printf(" info: changed SP%02d name from '%s' to '%s'\n", spidx + 1, from, to);
            return;
        }
    }

    printf(" warn: unable fo find species with name '%s'\n", from);

    return;
}
