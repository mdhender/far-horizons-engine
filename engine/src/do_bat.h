#ifndef FAR_HORIZONS_ENGINE_DO_BAT_H
#define FAR_HORIZONS_ENGINE_DO_BAT_H

void auto_enemy(int traitor_species_number, int betrayed_species_number);

void do_ambush(int ambushing_species_index, struct battle_data *bat);

void do_battle(struct battle_data *bat);

#endif //FAR_HORIZONS_ENGINE_DO_BAT_H
