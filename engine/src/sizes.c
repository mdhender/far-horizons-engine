#include <stdio.h>

#include "fh.h"

void SizeOf(char *s, size_t n, int w) {
        printf("%-20s is %6d want %6d\n", s, (int)n, w);
}

// we want
//   MAX_ITEMS           is     38
//   MAX_SPECIES         is    100
//   NUM_COMMANDS        is     54
//   NUM_CONTACT_WORDS   is      4
//   NUM_SHIP_CLASSES    is     18
//   sizeof char         is      1
//   sizeof long         is      4
//   sizeof short        is      2
//   sizeof galaxy_data  is     16
//   sizeof star_data    is     52
//   sizeof planet_data  is     40
//   sizeof nampla_data  is    288
//   sizeof species_data is    264
//   sizeof ship_data    is    172
//   sizeof trans_data   is    148
//
int main() {
        SizeOf("MAX_ITEMS", MAX_ITEMS, 38);
        SizeOf("MAX_SPECIES", MAX_SPECIES, 100);
        SizeOf("NUM_COMMANDS", NUM_COMMANDS, 54);
        SizeOf("NUM_CONTACT_WORDS", NUM_CONTACT_WORDS, 4);
        SizeOf("NUM_SHIP_CLASSES", NUM_SHIP_CLASSES, 18);
        SizeOf("sizeof char", sizeof(char), 1);
        SizeOf("sizeof int", sizeof(int), 4);
        SizeOf("sizeof long", sizeof(int32_t), 4); // long -> int32_t
        SizeOf("sizeof short", sizeof(int16_t), 2); // short -> int16_t
        SizeOf("sizeof galaxy_data", sizeof(struct galaxy_data), 16);
        SizeOf("sizeof star_data", sizeof(struct star_data), 52);
        SizeOf("sizeof planet_data", sizeof(struct planet_data), 40);
        SizeOf("sizeof nampla_data", sizeof(struct nampla_data), 288);
        SizeOf("sizeof species_data", sizeof(struct species_data), 264);
        SizeOf("sizeof ship_data", sizeof(struct ship_data), 172);
        SizeOf("sizeof trans_data", sizeof(struct trans_data), 148);

        return 0;
}
