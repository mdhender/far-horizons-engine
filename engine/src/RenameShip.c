#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "fh.h"

int
main(int argc, char *argv[]) {
    ssize_t              bytesRead;
    int                  changedShip;
    int                  fd;
    struct nampla_data * namp_data;
    char *               newName;
    char *               oldName;
    struct stat          sb;
    struct ship_data *   ship;
    char *               shipCode;
    struct ship_data *   ship_data;
    struct species_data *sp;
    struct species_data  spData;
    char                 spFile[128];
    int                  spNo;

    sp = &spData;

    if (argc != 4) {
        fprintf(stderr, "usage: RenameShip speciesNo oldName newName\n");
        fprintf(stderr, "where: speciesNo is an integer\n");
        fprintf(stderr, "     : oldName   is the name of the ship to update\n");
        fprintf(stderr, "     : newName   is the new name to use\n");
        fprintf(stderr, " note: if the ship name includes spaces, you must quote it\n");
        fprintf(stderr, " note: very picky about valid characters in names\n");
        fprintf(stderr, "   ex: ./RenameShip 18 'Mingo Pie' 'Mint Julep'\n");
        return(2);
    }

    spNo = atoi(argv[1]);
    if (spNo < 1 || spNo > 99) {
        fprintf(stderr, "error: '%s': speciesNo doesn't seem valid\n", argv[1]);
        return(2);
    }
    oldName = argv[2];
    newName = argv[3];

    /* verify the new name */
    if (strlen(newName) < 1) {
        fprintf(stderr, "error: '%s': new name is too short\n", newName);
        return(2);
    } else if (strlen(newName) > 32) {
        fprintf(stderr, "error: '%s': new name is too long\n", newName);
        return(2);
    } else if (strlen(newName) == 6) {
        char *cp = strdup(newName);
        for (char *s = cp; *s; s++) {
            *s = toupper(*s);
        }
        if (strcmp(cp, "UNUSED") == 0) {
            fprintf(stderr, "error: '%s': new name is forbidden\n", newName);
            return(2);
        }
    }
    for (char *s = newName; *s != 0; s++) {
        if (isalnum(*s)) {
            continue;
        } else if (*s == '\'' || *s == ' ' || *s == '.') {
            continue;
        }
        fprintf(stderr, "error: '%s': squeemish about character in new name\n", newName);
        return(2);
    }
    printf(" info: changing '%s' to '%s'\n", oldName, newName);

    sprintf(spFile, "sp%02d.dat", spNo);
    if (stat(spFile, &sb) != 0) {
        fprintf(stderr, "error: '%s': failed to stat\n", spFile);
        return(2);
    } else if (sb.st_size == 0) {
        fprintf(stderr, "error:'%s': there but empty\n", spFile);
        return(2);
    }

    printf(" info: opening '%s'\n", spFile);
    fd = open(spFile, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "error:'%s': can not open for reading\n", spFile);
        return(2);
    }
    bytesRead = read(fd, sp, sizeof(struct species_data));
    if (bytesRead != sizeof(struct species_data)) {
        fprintf(stderr, "error:'%s': can not read species data\n", spFile);
        return(2);
    }
    printf(" info: expecting %8d nampla records\n", sp->num_namplas);
    printf(" info: expecting %8d ship   records\n", sp->num_ships);
    if (sp->num_ships < 1) {
        fprintf(stderr, "error: no ships in input\n");
        return(2);
    }

    namp_data = (struct nampla_data *)calloc(sp->num_namplas, sizeof(struct nampla_data));
    if (namp_data == 0) {
        fprintf(stderr, "error: namp_data: can not allocate memory for %d records\n", sp->num_namplas);
        return(2);
    }
    bytesRead = read(fd, namp_data, sp->num_namplas * sizeof(struct nampla_data));
    if (bytesRead != sp->num_namplas * sizeof(struct nampla_data)) {
        fprintf(stderr, "error: namp_data: can not read %d records\n", sp->num_namplas);
        return(2);
    }
    printf(" info: loaded    %8d nampla records\n", sp->num_namplas);

    ship_data = (struct ship_data *)calloc(sp->num_ships, sizeof(struct ship_data));
    if (ship_data == 0) {
        fprintf(stderr, "error: ship_data: can not allocate memory for %d records\n", sp->num_ships);
        return(2);
    }
    bytesRead = read(fd, ship_data, sp->num_ships * sizeof(struct ship_data));
    if (bytesRead != sp->num_ships * sizeof(struct ship_data)) {
        fprintf(stderr, "error: ship_data: can not read %d records\n", sp->num_ships);
        return(2);
    }
    printf(" info: loaded    %8d ship   records\n", sp->num_ships);

    close(fd);

    printf(" info: changing '%s' to '%s'\n", oldName, newName);

    /* convert old name to uppercase for convenience */
    for (char *s = oldName; *s; s++) {
        *s = toupper(*s);
    }

    ship        = ship_data;
    changedShip = 0;
    for (int i = 0; i < sp->num_ships; i++) {
        char name[32];
        printf("ship: %3d: '%s'\n", i + 1, ship->name);
        for (int k = 0; k < 31; k++) {
            name[k] = ship->name[k];
            if (isalpha(name[k])) {
                name[k] = toupper(name[k]);
            }
        }
        name[31] = 0;
        if (strcmp(oldName, name) != 0) {
            ship++;
            continue;
        }
        printf(" info: changing '%s' to '%s'\n", ship->name, newName);
        for (int k = 0; k < strlen(newName); k++) {
            ship->name[k] = newName[k];
        }
        ship->name[strlen(newName)] = 0;
        printf("ship: %3d: '%s'\n", i + 1, ship->name);
        changedShip++;
        break;
    }

    if (changedShip == 0) {
        fprintf(stderr, "error: '%s': did not find ship\n", oldName);
        //return 2;
    }

    fd = open(spFile, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "error: '%s': can not write to file\n", spFile);
        return(2);
    }

    write(fd, sp, sizeof(struct species_data));
    write(fd, namp_data, sizeof(struct nampla_data) * sp->num_namplas);
    write(fd, ship_data, sizeof(struct ship_data) * sp->num_ships);
    close(fd);

    return(0);
}
