#include "fh.h"



extern int x, y, z, pn, num_stars, nampla_index,
           species_index;
extern char input_line[256], upper_name[32],
            original_name[32], *input_line_pointer;
extern FILE *log_file;
extern struct species_data *species;
extern struct star_data *   star;
extern struct planet_data * planet_base;
extern struct nampla_data * nampla_base, *nampla;

void
do_RENAME_command(void) {
    int i, found, name_length, unused_nampla_available;

    char *original_line_pointer;

    struct planet_data *planet;
    struct nampla_data *unused_nampla;


    // get x y z coordinates
    found = get_location();
    if (!found || nampla != NULL || pn == 0) {
        fprintf(log_file, "!!! Order ignored:\n");
        fprintf(log_file, "!!! %s", input_line);
        fprintf(log_file, "!!! Invalid coordinates in RENAME command.\n");
        return;
    }

    // get planet abbreviation
    skip_whitespace();
    original_line_pointer = input_line_pointer;
    if (get_class_abbr() != PLANET_ID) {
        fprintf(log_file, "!!! Order ignored:\n");
        fprintf(log_file, "!!! %s", input_line);
        fprintf(log_file, "!!! Invalid planet name in RENAME command.\n");
        return;
    }

    // get planet name
    name_length = get_name();
    if (name_length < 1) {
        fprintf(log_file, "!!! Order ignored:\n");
        fprintf(log_file, "!!! %s", input_line);
        fprintf(log_file, "!!! Invalid planet name in NAME command.\n");
        return;
    }

    // search existing namplas for name location
    for (nampla_index = 0; nampla_index < species->num_namplas; nampla_index++) {
        char upper_nampla_name[32];

        nampla = nampla_base + nampla_index;

        // make upper case copy of nampla name
        for (i = 0; i < 32; i++) {
            upper_nampla_name[i] = toupper(nampla->name[i]);
        }

        // compare names
        if (strcmp(upper_nampla_name, upper_name) == 0) {
            fprintf(log_file, "!!! Order ignored:\n");
            fprintf(log_file, "!!! %s", input_line);
            fprintf(log_file, "!!! The planet at the %d %d %d #%d already has this name.\n", x, y, z, pn);
            return;
        }
    }

    // search existing namplas for this location
    for (nampla_index = 0; nampla_index < species->num_namplas; nampla_index++) {
        nampla = nampla_base + nampla_index;

        // check if a named planet already exists at this location
        if (nampla->x == x && nampla->y == y && nampla->z == z && nampla->pn == pn) {
            // update record
            strcpy(nampla->name, original_name);
            // log result
            log_string("    Named PL ");  log_string(nampla->name);
            log_string(" at ");  log_int(nampla->x);  log_char(' ');
            log_int(nampla->y);  log_char(' ');  log_int(nampla->z);
            log_string(", planet #");  log_int(nampla->pn);
            log_string(".\n");
            return;
        }
    }

    fprintf(log_file, "!!! Order ignored:\n");
    fprintf(log_file, "!!! %s", input_line);
    fprintf(log_file, "!!! No named planet exists at %d %d %d #%d.\n", x, y, z, pn);

    return;
}
