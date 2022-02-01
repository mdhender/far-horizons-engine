#ifndef FAR_HORIZONS_ENGINE_UTILS_H
#define FAR_HORIZONS_ENGINE_UTILS_H

int agrep_score(char *correct_string, char *unknown_string);

int check_population(struct nampla_data *nampla);

void delete_ship(struct ship_data *ship);

int distorted(int species_number);

void log_message(char *message_filename);

long power(short tonnage);

int rnd(unsigned int max);

int undistorted(int distorted_species_number);

#endif //FAR_HORIZONS_ENGINE_UTILS_H
