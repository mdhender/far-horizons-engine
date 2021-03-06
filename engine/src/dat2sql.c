/*
 * Convert binary data files to portable sql.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

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

const char *commandCode(int j);
const char *gasCode(int j);
const char *itemCode(int j);
char *itoa(int val, char *buffer, int radix);
void jbool(FILE *fp, int indent, const char *name, int val, const char *eol);
void jcoords(FILE *fp, int indent, const char *name, int x, int y, int z, const char *eol);
void jint(FILE *fp, int indent, const char *name, int val, const char *eol);
void jstr(FILE *fp, int indent, const char *name, const char *val, const char *eol);
const char *mkkey(int x, int y, int z, int orbit);
const char *shipClassCode(int j);
const char *shipClassName(int j);
const char *shipStatusCode(int j);
const char *shipTypeCode(int j);
const char *starColor(int j);
const char *starType(int j);
const char *techCode(int j);
const char *transType(int j);
const char *strlower(const char *s);
char *strreverse(char *s);
const char *strupper(const char *s);

int
main(int argc, char *argv[]) {
    int  i, j, ok;
    char buffer[1024];

    FILE *fp;

    /* Get all binary data. */
    get_galaxy_data();
    get_star_data();
    get_planet_data();
    get_species_data();




    // create the output file
    sprintf(buffer, "galaxy.t%d.sql", galaxy.turn_number);
    fp = fopen(buffer, "w");
    if (fp == NULL) {
        fprintf(stderr, "Cannot create output file '%s'!\n", buffer);
        return(2);
    }

    fprintf(fp, "-- dat2sql version %s\n", semver);
    fprintf(fp, "create table if not exists galaxy(\n")
    fprintf(fp, "  turn_number   integer,\n");
    fprintf(fp, "  d_num_species integer,\n");
    fprintf(fp, "  num_species   integer,\n");
    fprintf(fp, "  radius        integer\n");
    fprintf(fp, ")\n");

    fprintf(fp, "insert into galaxy(turn_number, d_num_species, num_species, radius) values(%d, %d, %d, %d)\n", galaxy.turn_number, galaxy.d_num_species, galaxy.num_species, galaxy.radius);

    fprintf(fp, "create table if not exists systems(\n")
    fprintf(fp, "  id          integer,\n");
    fprintf(fp, "  star_type   text,\n")
    fprintf(fp, "  star_color  text,\n")
    fprintf(fp, "  star_size   integer,\n")
    fprintf(fp, "  x           integer,\n");
    fprintf(fp, "  y           integer,\n");
    fprintf(fp, "  z           integer,\n");
    fprintf(fp, "  num_planets integer,\n");
    fprintf(fp, "  home_system text,\n");
    fprintf(fp, "  wormhole    text\n");
    fprintf(fp, ")\n");

    fprintf(fp, "create table if not exists wormholes(\n")
    fprintf(fp, "  id          integer,\n");
    fprintf(fp, "  x           integer,\n");
    fprintf(fp, "  y           integer,\n");
    fprintf(fp, "  z           integer\n");
    fprintf(fp, ")\n");

    fprintf(fp, "create table if not exists planets(\n")
    fprintf(fp, "  id                integer,\n");
    fprintf(fp, "  orbit             integer,\n");
    fprintf(fp, "  temperature_class integer,\n");
    fprintf(fp, "  pressure_class    integer,\n");
    fprintf(fp, "  diameter          integer,\n");
    fprintf(fp, "  gravity           integer,\n");
    fprintf(fp, "  mining_difficulty integer,\n");
    fprintf(fp, "  econ_efficiency   integer,\n");
    fprintf(fp, "  md_increase       integer,\n");
    fprintf(fp, "  message           integer\n");
    fprintf(fp, ")\n");

    fprintf(fp, "create table if not exists visited_by(\n")
    fprintf(fp, "  id          integer,\n");
    fprintf(fp, "  species     integer\n");
    fprintf(fp, ")\n");

    fprintf(fp, "create table if not exists messages(\n")
    fprintf(fp, "  id          integer,\n");
    fprintf(fp, "  kind        text,\n");
    fprintf(fp, "  message     integer\n");
    fprintf(fp, ")\n");

    if (galaxy.turn_number == 1) {
        for (i = 0; i < num_stars; i++) {
            star = star_base + i;
            const char *home_system = star->home_system != 0 ? "Y" : "N";
            const char *wormhole = star->worm_here ? "Y" : "N";
            fprintf(fp, "insert into systems(id, star_type, star_color, star_size, x, y, z, num_planets, home_system, wormhole) values(%d, '%s', '%s', %d, %d, %d, %d, %d, '%s', '%s')\n", i+1, star->type, star->color, star->size, star->x, star->y, star->z, star->num_planets, home_system, wormhole);

            if (star->worm_here != 0) {
                fprintf(fp, "insert into wormholes(id, x, y, z) values(%d, %d, %d, %d)\n", i+1, star->worm_x, star->worm_y, star->worm_z);
            }

            for (int orbit = 0; orbit < star->num_planets; orbit++) {
                fprintf(fp, "insert into planets(id, orbit) values (%d, %d)\n", i+1, orbit+1);
            }

            for (j = 0; j < NUM_CONTACT_WORDS; j++) {
                if (star->visited_by[j] != 0) {
                    char *vsep = "";
                    for (int sp = 1; sp <= MAX_SPECIES; sp++) {
                        /* Get array index and bit mask. */
                        int  species_array_index, species_bit_number;
                        long species_bit_mask;
                        species_array_index = (sp - 1) / 32;
                        species_bit_number  = (sp - 1) % 32;
                        species_bit_mask    = 1 << species_bit_number;
                        /* Check if bit is already set. */
                        if (star->visited_by[species_array_index] & species_bit_mask) {
                            fprintf(fp, "insert into visited_by(id, species) (%d, %d)\n", i+1, sp);
                        }
                    }
                    break;
                }
            }

            if (star->message) != 0 {
                fprintf(fp, "insert into messages(id, kind, message) values(%d, 'SYSTEM', %d)\n", i+1, star->message);
            }
        }
    }

    fprintf(fp, "\t\"planets\": [\n");
    for (i = 0; i < num_planets; i++) {
        const char *gsep = "";
        struct planet_data *planet = planet_base + i;
        fprintf(fp, "\t\t{\n");
        jint(fp, 3, "id", i + 1, ",\n");
        jint(fp, 3, "temperature_class", planet->temperature_class, ",\n");
        jint(fp, 3, "pressure_class", planet->pressure_class, ",\n");
        fprintf(fp, "\t\t\t\"gases\": {");
        gsep = "";
        for (j = 0; j < 4; j++) {
            if (planet->gas[j] == 0 || planet->gas_percent[j] == 0) {
                continue;
            }
            fprintf(fp, "%s\"%s\": %d", gsep, gasCode(planet->gas[j]), planet->gas_percent[j]);
            gsep = ", ";
        }
        fprintf(fp, "},\n");
        jint(fp, 3, "diameter", planet->diameter, ",\n");
        jint(fp, 3, "gravity", planet->gravity, ",\n");
        jint(fp, 3, "mining_difficulty", planet->mining_difficulty, ",\n");
        jint(fp, 3, "econ_efficiency", planet->econ_efficiency, ",\n");
        jint(fp, 3, "md_increase", planet->md_increase, ",\n");
        jint(fp, 3, "message", planet->message, "\n");
        fprintf(fp, "\t\t}");
        if (i + 1 < num_planets) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\t],\n");

    fprintf(fp, "\t\"species\": {\n");
    for (species_index = 0; species_index < galaxy.num_species; species_index++) {
        const char *gsep = "";
        species_number = species_index + 1;

        fprintf(fp, "\t\t\"SP%02d\": {\n", species_number);
        jint(fp, 3, "id", species_number, ",\n");

        if (!data_in_memory[species_index]) {
            fprintf(fp, "\t\t}");
            if (i + 1 < galaxy.num_species) {
                fprintf(fp, ",");
            }
            fprintf(fp, "\n");
            printf("Skipping species #%d.\n", species_number);
            continue;
        }

        species     = &spec_data[species_number - 1];
        nampla_base = namp_data[species_number - 1];
        ship_base   = ship_data[species_number - 1];

        jstr(fp, 3, "name", species->name, ",\n");
        fprintf(fp, "\t\t\t\"government\": {\n");
        jstr(fp, 4, "name", species->govt_name, ",\n");
        jstr(fp, 4, "type", species->govt_type, "\n");
        fprintf(fp, "\t\t\t},\n");
        fprintf(fp, "\t\t\t\"homeworld\": {\n");
        jstr(fp, 4, "key", mkkey(species->x, species->y, species->z, species->pn), ",\n");
        jcoords(fp, 4, "coords", species->x, species->y, species->z, ",\n");
        jint(fp, 4, "orbit", species->pn, "\n");
        fprintf(fp, "\t\t\t},\n");
        fprintf(fp, "\t\t\t\"gases\": {\n");
        fprintf(fp, "\t\t\t\t\"required\": {\"%s\": {\"min\": %d, \"max\": %d}},\n", gasCode(species->required_gas), species->required_gas_min, species->required_gas_max);
        fprintf(fp, "\t\t\t\t\"poison\": {");
        gsep = "";
        for (j = 0; j < 6; j++) {
            fprintf(fp, "%s\"%s\": true", gsep, gasCode(species->poison_gas[j]));
            gsep = ", ";
        }
        fprintf(fp, "}\n");
        fprintf(fp, "\t\t\t},\n");
        if (species->auto_orders != 0) {
            jbool(fp, 3, "auto_orders", species->auto_orders, ",\n");
        }
        fprintf(fp, "\t\t\t\"tech\": {\n");
        fprintf(fp, "\t\t\t\t\"MI\": {\"level\": %d, \"init\": %d, \"knowledge\": %d, \"xp\": %ld},\n", species->tech_level[0], species->init_tech_level[0], species->tech_knowledge[0], (long int)(species->tech_eps[0]));
        fprintf(fp, "\t\t\t\t\"MA\": {\"level\": %d, \"init\": %d, \"knowledge\": %d, \"xp\": %ld},\n", species->tech_level[1], species->init_tech_level[1], species->tech_knowledge[1], (long int)(species->tech_eps[1]));
        fprintf(fp, "\t\t\t\t\"ML\": {\"level\": %d, \"init\": %d, \"knowledge\": %d, \"xp\": %ld},\n", species->tech_level[2], species->init_tech_level[2], species->tech_knowledge[2], (long int)(species->tech_eps[2]));
        fprintf(fp, "\t\t\t\t\"GV\": {\"level\": %d, \"init\": %d, \"knowledge\": %d, \"xp\": %ld},\n", species->tech_level[3], species->init_tech_level[3], species->tech_knowledge[3], (long int)(species->tech_eps[3]));
        fprintf(fp, "\t\t\t\t\"LS\": {\"level\": %d, \"init\": %d, \"knowledge\": %d, \"xp\": %ld},\n", species->tech_level[4], species->init_tech_level[4], species->tech_knowledge[4], (long int)(species->tech_eps[4]));
        fprintf(fp, "\t\t\t\t\"BI\": {\"level\": %d, \"init\": %d, \"knowledge\": %d, \"xp\": %ld}\n", species->tech_level[5], species->init_tech_level[5], species->tech_knowledge[5], (long int)(species->tech_eps[5]));
        fprintf(fp, "\t\t\t},\n");
        jint(fp, 3, "econ_units", species->econ_units, ",\n");
        jint(fp, 3, "hp_original_base", species->hp_original_base, ",\n");
        jint(fp, 3, "fleet_cost", species->fleet_cost, ",\n");
        jint(fp, 3, "fleet_percent_cost", species->fleet_percent_cost, ",\n");

        fprintf(fp, "\t\t\t\"contacts\": [");
        for (j = 0; j < NUM_CONTACT_WORDS; j++) {
            if (species->contact[j] != 0) {
                char *vsep = "";
                for (int sp = 1; sp <= MAX_SPECIES; sp++) {
                    /* Get array index and bit mask. */
                    int  species_array_index, species_bit_number;
                    long species_bit_mask;
                    species_array_index = (sp - 1) / 32;
                    species_bit_number  = (sp - 1) % 32;
                    species_bit_mask    = 1 << species_bit_number;
                    /* Check if bit is already set. */
                    if (species->contact[species_array_index] & species_bit_mask) {
                        fprintf(fp, "%s\"SP%02d\"", vsep, sp);
                        vsep = ", ";
                    }
                }
                break;
            }
        }
        fprintf(fp, "],\n");
        fprintf(fp, "\t\t\t\"allies\": [");
        for (j = 0; j < NUM_CONTACT_WORDS; j++) {
            if (species->ally[j] != 0) {
                char *vsep = "";
                for (int sp = 1; sp <= MAX_SPECIES; sp++) {
                    /* Get array index and bit mask. */
                    int  species_array_index, species_bit_number;
                    long species_bit_mask;
                    species_array_index = (sp - 1) / 32;
                    species_bit_number  = (sp - 1) % 32;
                    species_bit_mask    = 1 << species_bit_number;
                    /* Check if bit is already set. */
                    if (species->ally[species_array_index] & species_bit_mask) {
                        fprintf(fp, "%s\"SP%02d\"", vsep, sp);
                        vsep = ", ";
                    }
                }
                break;
            }
        }
        fprintf(fp, "],\n");
        fprintf(fp, "\t\t\t\"enemies\": [");
        for (j = 0; j < NUM_CONTACT_WORDS; j++) {
            if (species->enemy[j] != 0) {
                char *vsep = "";
                for (int sp = 1; sp <= MAX_SPECIES; sp++) {
                    /* Get array index and bit mask. */
                    int  species_array_index, species_bit_number;
                    long species_bit_mask;
                    species_array_index = (sp - 1) / 32;
                    species_bit_number  = (sp - 1) % 32;
                    species_bit_mask    = 1 << species_bit_number;
                    /* Check if bit is already set. */
                    if (species->enemy[species_array_index] & species_bit_mask) {
                        fprintf(fp, "%s\"SP%02d\"", vsep, sp);
                        vsep = ", ";
                    }
                }
                break;
            }
        }
        fprintf(fp, "],\n");

        fprintf(fp, "\t\t\t\"namplas\": {\n");
        for (i = 0; i < species->num_namplas; i++) {
            const char *isep = "";
            nampla = nampla_base + i;

            fprintf(fp, "\t\t\t\t\"%s\": {\n", strupper(nampla->name));
            jint(fp, 5, "id", i + 1, ",\n");
            jstr(fp, 5, "name", nampla->name, ",\n");
            jstr(fp, 5, "location", mkkey(nampla->x, nampla->y, nampla->z, nampla->pn), ",\n");
            jcoords(fp, 5, "coords", nampla->x, nampla->y, nampla->z, ",\n");
            jint(fp, 5, "orbit", nampla->pn, ",\n");
            fprintf(fp, "\t\t\t\t\t\"status\": {\n");
            jbool(fp, 6, "home_planet", nampla->status & HOME_PLANET, ",\n");
            jbool(fp, 6, "colony", nampla->status & COLONY, ",\n");
            jbool(fp, 6, "populated", nampla->status & POPULATED, ",\n");
            jbool(fp, 6, "mining_colony", nampla->status & MINING_COLONY, ",\n");
            jbool(fp, 6, "resort_colony", nampla->status & RESORT_COLONY, ",\n");
            jbool(fp, 6, "disbanded_colony", nampla->status & DISBANDED_COLONY, "\n");
            fprintf(fp, "\t\t\t\t\t},\n");
            jbool(fp, 5, "hiding", nampla->hiding, ",\n");
            jint(fp, 5, "hidden", nampla->hidden, ",\n");
            jint(fp, 5, "planet_index", nampla->planet_index, ",\n");
            jint(fp, 5, "siege_eff", nampla->siege_eff, ",\n");
            jint(fp, 5, "shipyards", nampla->shipyards, ",\n");
            jint(fp, 5, "IUs_needed", nampla->IUs_needed, ",\n");
            jint(fp, 5, "AUs_needed", nampla->AUs_needed, ",\n");
            jint(fp, 5, "auto_IUs", nampla->auto_IUs, ",\n");
            jint(fp, 5, "auto_AUs", nampla->auto_AUs, ",\n");
            jint(fp, 5, "IUs_to_install", nampla->IUs_to_install, ",\n");
            jint(fp, 5, "AUs_to_install", nampla->AUs_to_install, ",\n");
            jint(fp, 5, "mi_base", nampla->mi_base, ",\n");
            jint(fp, 5, "ma_base", nampla->ma_base, ",\n");
            jint(fp, 5, "pop_units", nampla->pop_units, ",\n");
            jint(fp, 5, "use_on_ambush", nampla->use_on_ambush, ",\n");
            jint(fp, 5, "message", nampla->message, ",\n");
            fprintf(fp, "\t\t\t\t\t\"inventory\": {");
            isep = "";
            for (j = 0; j < MAX_ITEMS; j++) {
                if (nampla->item_quantity[j] == 0) {
                    continue;
                }
                fprintf(fp, "%s\"%s\": %ld", isep, itemCode(j), (long int)(nampla->item_quantity[j]));
                isep = ", ";
            }
            fprintf(fp, "}\n");
            fprintf(fp, "\t\t\t\t}");
            if (i + 1 < species->num_namplas) {
                fprintf(fp, ",");
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\t\t\t},\n");

        /* Do ships for this species. */
        fprintf(fp, "\t\t\t\"ships\": {\n");
        for (i = 0; i < species->num_ships; i++) {
            const char *isep = "";
            ship = ship_base + i;

            fprintf(fp, "\t\t\t\t\"%s\": {\n", strupper(ship->name));
            jint(fp, 5, "id", i + 1, ",\n");
            jstr(fp, 5, "name", ship->name, ",\n");
            jstr(fp, 5, "location", mkkey(ship->x, ship->y, ship->z, ship->pn), ",\n");
            jcoords(fp, 5, "coords", ship->x, ship->y, ship->z, ",\n");
            jint(fp, 5, "orbit", ship->pn, ",\n");
            jstr(fp, 5, "class", shipClassCode(ship->class), ",\n");
            jstr(fp, 5, "type", shipTypeCode(ship->type), ",\n");
            jint(fp, 5, "tonnage", ship->tonnage, ",\n");
            jint(fp, 5, "age", ship->age, ",\n");
            jstr(fp, 5, "status", shipStatusCode(ship->status), ",\n");
            jcoords(fp, 5, "dest", ship->dest_x, ship->dest_y, ship->dest_z, ",\n");
            if (ship->just_jumped != 0) {
                jbool(fp, 5, "just_jumped", ship->just_jumped, ",\n");
            }
            if (ship->arrived_via_wormhole != 0) {
                jbool(fp, 5, "arrived_via_wormhole", ship->arrived_via_wormhole, ",\n");
            }
            jint(fp, 5, "loading_point", ship->loading_point, ",\n");
            jint(fp, 5, "unloading_point", ship->unloading_point, ",\n");
            jint(fp, 5, "remaining_cost", ship->remaining_cost, ",\n");
            jint(fp, 5, "message", 0, ",\n");
            fprintf(fp, "\t\t\t\t\t\"inventory\": {");
            for (j = 0; j < MAX_ITEMS; j++) {
                if (ship->item_quantity[j] == 0) {
                    continue;
                }
                fprintf(fp, "%s\"%s\": %d", isep, itemCode(j), ship->item_quantity[j]);
                isep = ", ";
            }
            fprintf(fp, "}\n");
            fprintf(fp, "\t\t\t\t}");
            if (i + 1 < species->num_ships) {
                fprintf(fp, ",");
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\t\t\t}\n");

        fprintf(fp, "\t\t}");
        if (species_index + 1 < galaxy.num_species) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\t},\n");

    fprintf(fp, "\t\"commands\": {\n");
    for (int i = 1; i < NUM_COMMANDS; i++) {
        jstr(fp, 2, command_abbr[i], command_name[i], ",\n");
    }
    jstr(fp, 2, command_abbr[0], command_name[0], "\n");
    fprintf(fp, "\t},\n");

    fprintf(fp, "\t\"items\": {\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        fprintf(fp, "\t\t\"%s\": {\n", item_abbr[i]);
        jstr(fp, 3, "name", item_name[i], ",\n");
        jint(fp, 3, "cost", item_cost[i], ",\n");
        if (item_critical_tech[i] >= MI && item_critical_tech[i] <= BI) {
            fprintf(fp, "\t\t\t\"tech\": {\"%s\": %d},\n", techCode(item_critical_tech[i]), item_tech_requirment[i]);
        } else {
        }
        jint(fp, 3, "carry_cost", item_carry_capacity[i], "\n");
        fprintf(fp, "\t\t}");
        if (i + 1 < MAX_ITEMS) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\t},\n");

    fprintf(fp, "\t\"ships\": {\n");
    for (int i = 0; i < NUM_SHIP_CLASSES; i++) {
        fprintf(fp, "\t\t\"%s\": {\n", ship_abbr[i]);
        jstr(fp, 3, "class", shipClassName(i), ",\n");
        jint(fp, 3, "min_ma", 2 * ship_tonnage[i], ",\n");
        jint(fp, 3, "cost_ftl", ship_cost[i], ",\n");
        jint(fp, 3, "cost_sublight", 75 * ship_cost[i] / 100, ",\n");
        if (i == TR) {
            jint(fp, 3, "tonnage", ship_tonnage[i], "\n");
        } else {
            jint(fp, 3, "tonnage", ship_tonnage[i], ",\n");
            jint(fp, 3, "carrying_capacity", ship_tonnage[i], "\n");
        }
        fprintf(fp, "\t\t}");
        if (i + 1 < NUM_SHIP_CLASSES) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\t},\n");

    fprintf(fp, "\t\"tech\": {\n");
    for (int i = MI; i <= BI; i++) {
        const char *eol = ",\n";
        if (i == BI) {
            eol = "\n";
        }
        jstr(fp, 2, tech_abbr[i], tech_name[i], eol);
    }
    fprintf(fp, "\t}\n");

    fprintf(fp, "}\n");
    fclose(fp);

    return(0);
}

const char *
commandCode(int j) {
    switch (j) {
    case UNDEFINED : return "UNDEFINED";
    case ALLY : return "ALLY";
    case AMBUSH : return "AMBUSH";
    case ATTACK : return "ATTACK";
    case AUTO : return "AUTO";
    case BASE : return "BASE";
    case BATTLE : return "BATTLE";
    case BUILD : return "BUILD";
    case CONTINUE : return "CONTINUE";
    case DEEP : return "DEEP";
    case DESTROY : return "DESTROY";
    case DEVELOP : return "DEVELOP";
    case DISBAND : return "DISBAND";
    case END : return "END";
    case ENEMY : return "ENEMY";
    case ENGAGE : return "ENGAGE";
    case ESTIMATE : return "ESTIMATE";
    case HAVEN : return "HAVEN";
    case HIDE : return "HIDE";
    case HIJACK : return "HIJACK";
    case IBUILD : return "IBUILD";
    case ICONTINUE : return "ICONTINUE";
    case INSTALL : return "INSTALL";
    case INTERCEPT : return "INTERCEPT";
    case JUMP : return "JUMP";
    case LAND : return "LAND";
    case MESSAGE : return "MESSAGE";
    case MOVE : return "MOVE";
    case NAME : return "NAME";
    case NEUTRAL : return "NEUTRAL";
    case ORBIT : return "ORBIT";
    case PJUMP : return "PJUMP";
    case PRODUCTION : return "PRODUCTION";
    case RECYCLE : return "RECYCLE";
    case REPAIR : return "REPAIR";
    case RESEARCH : return "RESEARCH";
    case SCAN : return "SCAN";
    case SEND : return "SEND";
    case SHIPYARD : return "SHIPYARD";
    case START : return "START";
    case SUMMARY : return "SUMMARY";
    case SURRENDER : return "SURRENDER";
    case TARGET : return "TARGET";
    case TEACH : return "TEACH";
    case TECH : return "TECH";
    case TELESCOPE : return "TELESCOPE";
    case TERRAFORM : return "TERRAFORM";
    case TRANSFER : return "TRANSFER";
    case UNLOAD : return "UNLOAD";
    case UPGRADE : return "UPGRADE";
    case VISITED : return "VISITED";
    case WITHDRAW : return "WITHDRAW";
    case WORMHOLE : return "WORMHOLE";
    case ZZZ : return "ZZZ";
    }
    abend("assert(command_code != %d)\n", j);
    return "??";
}

/* Gases in planetary atmospheres. */
const char *
gasCode(int j) {
    switch (j) {
    case H2 : return "H2";  // Hydrogen
    case CH4: return "CH4";   // Methane
    case HE : return "He";  // Helium
    case NH3: return "NH3";   // Ammonia
    case N2 : return "N2";  // Nitrogen
    case CO2: return "CO2";   // Carbon Dioxide
    case O2 : return "O2";  // Oxygen
    case HCL: return "HCl";   // Hydrogen Chloride
    case CL2: return "Cl2";   // Chlorine
    case F2 : return "F2";   // Fluorine
    case H2O: return "H2O";    // Steam
    case SO2: return "SO2";    // Sulfur Dioxide
    case H2S: return "H2S";    // Hydrogen Sulfide
    }
    abend("assert(gas_code != %d)\n", j);
    return("??");
}

const char *
itemCode(int j) {
    switch (j) {
    case RM:  return("RM");  /* Raw Material Units. */

    case PD:  return("PD");  /* Planetary Defense Units. */

    case SU:  return("SU");  /* Starbase Units. */

    case DR:  return("DR");  /* Damage Repair Units. */

    case CU:  return("CU");  /* Colonist Units. */

    case IU:  return("IU");  /* Colonial Mining Units. */

    case AU:  return("AU");  /* Colonial Manufacturing Units. */

    case FS:  return("FS");  /* Fail-Safe Jump Units. */

    case JP:  return("JP");  /* Jump Portal Units. */

    case FM:  return("FM");  /* Forced Misjump Units. */

    case FJ:  return("FJ");  /* Forced Jump Units. */

    case GT:  return("GT");  /* Gravitic Telescope Units. */

    case FD:  return("FD");  /* Field Distortion Units. */

    case TP:  return("TP");  /* Terraforming Plants. */

    case GW:  return("GW");  /* Germ Warfare Bombs. */

    case SG1: return("SG1"); /* Mark-1 Auxiliary Shield Generators. */

    case SG2: return("SG2"); /* Mark-2. */

    case SG3: return("SG3"); /* Mark-3. */

    case SG4: return("SG4"); /* Mark-4. */

    case SG5: return("SG5"); /* Mark-5. */

    case SG6: return("SG6"); /* Mark-6. */

    case SG7: return("SG7"); /* Mark-7. */

    case SG8: return("SG8"); /* Mark-8. */

    case SG9: return("SG9"); /* Mark-9. */

    case GU1: return("GU1"); /* Mark-1 Auxiliary Gun Units. */

    case GU2: return("GU2"); /* Mark-2. */

    case GU3: return("GU3"); /* Mark-3. */

    case GU4: return("GU4"); /* Mark-4. */

    case GU5: return("GU5"); /* Mark-5. */

    case GU6: return("GU6"); /* Mark-6. */

    case GU7: return("GU7"); /* Mark-7. */

    case GU8: return("GU8"); /* Mark-8. */

    case GU9: return("GU9"); /* Mark-9. */

    case X1:  return("X1");  /* Unassigned. */

    case X2:  return("X2");  /* Unassigned. */

    case X3:  return("X3");  /* Unassigned. */

    case X4:  return("X4");  /* Unassigned. */

    case X5:  return("X5");  /* Unassigned. */
    }
    abend("assert(item_index != %d)\n", j);
    return("??");
}

// itoa from K&R
char *
itoa(int val, char *buffer, int radix) {
    static const char *digits = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

    if (val != 0 && 2 <= radix && radix <= 36) {
        int sign = val;
        int len  = 0;
        for ( ; val != 0;) {
            int tmp = val;
            val        /= radix;
            buffer[len] = digits[35 + (tmp - val * radix)];
            len++;
        }
        ;

        if (sign < 0) {
            buffer[len] = '-';
            len++;
        }
        buffer[len] = 0;

        return(strreverse(buffer));
    }

    buffer[0] = '0';
    buffer[1] = 0;
    return(buffer);
}

void
jbool(FILE *fp, int indent, const char *name, int val, const char *eol) {
    for (int i = 0; i < indent; i++) {
        fputc('\t', fp);
    }
    ;
    if (val == 0) {
        fprintf(fp, "\"%s\": false%s", name, eol);
    } else {
        fprintf(fp, "\"%s\": true%s", name, eol);
    }
}

void
jcoords(FILE *fp, int indent, const char *name, int x, int y, int z, const char *eol) {
    for (int i = 0; i < indent; i++) {
        fputc('\t', fp);
    }
    ;
    fprintf(fp, "\"%s\": {\"x\": %d, \"y\": %d, \"z\": %d}%s", name, x, y, z, eol);
}

void
jint(FILE *fp, int indent, const char *name, int val, const char *eol) {
    for (int i = 0; i < indent; i++) {
        fputc('\t', fp);
    }
    ;
    fprintf(fp, "\"%s\": %d%s", name, val, eol);
}

void
jstr(FILE *fp, int indent, const char *name, const char *val, const char *eol) {
    for (int i = 0; i < indent; i++) {
        fputc('\t', fp);
    }
    ;
    if (val == 0) {
        fprintf(fp, "\"%s\": \"\"%s", name, eol);
    } else {
        fprintf(fp, "\"%s\": \"%s\"%s", name, val, eol);
    }
}

const char *
mkkey(int x, int y, int z, int orbit) {
    static char key[64];

    if (orbit == 0) {
        sprintf(key, "%d %d %d", x, y, z);
    } else {
        sprintf(key, "%d %d %d #%d", x, y, z, orbit);
    }
    return(key);
}

/* Ship classes. */
const char *
shipClassCode(int j) {
    switch (j) {
    case PB: return("PB");   /* Picketboat. */

    case CT: return("CT");   /* Corvette. */

    case ES: return("ES");   /* Escort. */

    case DD: return("DD");   /* Destroyer. */

    case FG: return("FG");   /* Frigate. */

    case CL: return("CL");   /* Light Cruiser. */

    case CS: return("CS");   /* Strike Cruiser. */

    case CA: return("CA");   /* Heavy Cruiser. */

    case CC: return("CC");   /* Command Cruiser. */

    case BC: return("BC");   /* Battlecruiser. */

    case BS: return("BS");   /* Battleship. */

    case DN: return("DN");   /* Dreadnought. */

    case SD: return("SD");   /* Super Dreadnought. */

    case BM: return("BM");   /* Battlemoon. */

    case BW: return("BW");   /* Battleworld. */

    case BR: return("BR");   /* Battlestar. */

    case BA: return("BA");   /* Starbase. */

    case TR: return("TR");   /* Transport. */
    }
    abend("assert(ship_class != %d)\n", j);
    return("??");
}

const char *
shipClassName(int j) {
    switch (j) {
    case PB: return("Picketboat");
    case CT: return("Corvette");
    case ES: return("Escort");
    case DD: return("Destroyer");
    case FG: return("Frigate");
    case CL: return("Light Cruiser");
    case CS: return("Strike Cruiser");
    case CA: return("Heavy Cruiser");
    case CC: return("Command Cruiser");
    case BC: return("Battlecruiser");
    case BS: return("Battleship");
    case DN: return("Dreadnought");
    case SD: return("Super Dreadnought");
    case BM: return("Battlemoon");
    case BW: return("Battleworld");
    case BR: return("Battlestar");
    case BA: return("Starbase");
    case TR: return("Transport");
    }
    abend("assert(ship_class != %d)\n", j);
    return("??");
}

const char *
shipStatusCode(int j) {
    switch (j) {
    case UNDER_CONSTRUCTION: return("UNDER_CONSTRUCTION");

    case ON_SURFACE: return("ON_SURFACE");

    case IN_ORBIT: return("IN_ORBIT");

    case IN_DEEP_SPACE: return("IN_DEEP_SPACE");

    case JUMPED_IN_COMBAT: return("JUMPED_IN_COMBAT");

    case FORCED_JUMP: return("FORCED_JUMP");
    }
    abend("assert(ship_status != %d)\n", j);
    return("??");
}

const char *
shipTypeCode(int j) {
    switch (j) {
    case FTL: return("FTL");

    case SUB_LIGHT: return("SUB_LIGHT");

    case STARBASE: return("STARBASE");
    }
    abend("assert(ship_type != %d)\n", j);
    return("??");
}

const char *
starColor(int j) {
    switch (j) {
    case BLUE          : return "BLUE";
    case BLUE_WHITE          : return "BLUE_WHITE";
    case WHITE          : return "WHITE";
    case YELLOW_WHITE          : return "YELLOW_WHITE";
    case YELLOW          : return "YELLOW";
    case ORANGE          : return "ORANGE";
    case RED          : return "RED";
    }
    abend("assert(star_color != %d)\n", j);
    return "??";
}

const char *
starColorCode(int j) {
    switch (j) {
    case BLUE          : return "O";
    case BLUE_WHITE          : return "B";
    case WHITE          : return "A";
    case YELLOW_WHITE          : return "F";
    case YELLOW          : return "G";
    case ORANGE          : return "K";
    case RED          : return "M";
    }
    return " ";
}

const char *
starType(int j) {
    switch (j) {
    case DWARF          : return "DWARF";
    case DEGENERATE     : return "DEGENERATE";
    case MAIN_SEQUENCE  : return "MAIN_SEQUENCE";
    case GIANT          : return "GIANT";
    }
    abend("assert(star_type != %d)\n", j);
    return "??";
}

const char *
starTypeCode(int j) {
    switch (j) {
    case DWARF          : return "d";
    case DEGENERATE     : return "D";
    case MAIN_SEQUENCE  : return " ";
    case GIANT          : return "g";
    }
    return " ";
}

const char *
strlower(const char *s) {
    static char buffer[128];
    int i;
    for (i = 0; i < 127 && *s != 0; i++) {
        buffer[i] = tolower(*s);
        s++;
    }
    buffer[i] = 0;
    return buffer;
}


// reverse the buffer by swapping from the ends
char *
strreverse(char *s) {
    char *begin = s;

    for (char *end = begin + strlen(begin) - 1; end > begin; end--) {
        char ch = *begin;
        *begin = *end;
        *end   = ch;
        begin++;
    }
    return(s);
}

const char *
strupper(const char *s) {
    static char buffer[128];
    int i;
    for (i = 0; i < 127 && *s != 0; i++) {
        buffer[i] = toupper(*s);
        s++;
    }
    buffer[i] = 0;
    return buffer;
}

const char *
techCode(int j) {
    switch (j) {
    case BI: return "BI";
    case GV: return "GV";
    case LS: return "LS";
    case MA: return "MA";
    case MI: return "MI";
    case ML: return "ML";
    }
    abend("assert(tech_code != %d)\n", j);
    return "??";
}

const char *
transType(int j) {
    switch (j) {
    case EU_TRANSFER : return "EU_TRANSFER";
    case MESSAGE_TO_SPECIES : return "MESSAGE_TO_SPECIES";
    case BESIEGE_PLANET : return "BESIEGE_PLANET";
    case SIEGE_EU_TRANSFER : return "SIEGE_EU_TRANSFER";
    case TECH_TRANSFER : return "TECH_TRANSFER";
    case DETECTION_DURING_SIEGE : return "DETECTION_DURING_SIEGE";
    case SHIP_MISHAP : return "SHIP_MISHAP";
    case ASSIMILATION : return "ASSIMILATION";
    case INTERSPECIES_CONSTRUCTION : return "INTERSPECIES_CONSTRUCTION";
    case TELESCOPE_DETECTION : return "TELESCOPE_DETECTION";
    case ALIEN_JUMP_PORTAL_USAGE : return "ALIEN_JUMP_PORTAL_USAGE";
    case KNOWLEDGE_TRANSFER : return "KNOWLEDGE_TRANSFER";
    case LANDING_REQUEST : return "LANDING_REQUEST";
    case LOOTING_EU_TRANSFER : return "LOOTING_EU_TRANSFER";
    case ALLIES_ORDER : return "ALLIES_ORDER";
    }
    abend("assert(trans_type != %d)\n", j);
    return "??";
}
